
/* PostgreSQL headers */
#include "postgres.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "utils/memutils.h"
#include "executor/spi.h"
#include "access/hash.h"
#include "utils/hsearch.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

/* standardizer headers */
#undef DEBUG
//#define DEBUG 1

#include "std_pg_hash.h"

/* C headers */
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifdef DEBUG
#include <stdio.h>
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif



#ifdef DEBUG
#define SET_TIME(a) gettimeofday(&(a), NULL)
#define ELAPSED_T(a,b) \
    elapsed = (b.tv_sec - a.tv_sec)*1000.0; \
    elapsed += (b.tv_usec - a.tv_usec)/1000.0;
#else
#define SET_TIME(a) do { ; } while (0)
#define ELAPSED_T(a,b) do { ; } while (0)
#endif

#define MAX_RULE_LENGTH 128
#define TUPLIMIT 1000

#define STD_CACHE_ITEMS 4
#define STD_BACKEND_HASH_SIZE 16

static HTAB* StdHash = NULL;


typedef struct
{
    char *lex_md5;
    char *gmr_md5;
    STANDARDIZER *std;
    MemoryContext std_mcxt;
}
StdCacheItem;

typedef struct
{
    StdCacheItem StdCache[STD_CACHE_ITEMS];
    int NextSlot;
    MemoryContext StdCacheContext;
}
StdPortalCache;

typedef struct
{
    MemoryContext context;
    STANDARDIZER *std;
}
StdHashEntry;


/* Memory context hash table function prototypes */
uint32 mcxt_ptr_hash_std(const void *key, Size keysize);
static void CreateStdHash(void);
static void AddStdHashEntry(MemoryContext mcxt, STANDARDIZER *std);
static StdHashEntry *GetStdHashEntry(MemoryContext mcxt);
static void DeleteStdHashEntry(MemoryContext mcxt);

/* Memory context cache function prototypes */
static void StdCacheInit(MemoryContext context);
static void StdCacheReset(MemoryContext context);
static void StdCacheDelete(MemoryContext context);
static bool StdCacheIsEmpty(MemoryContext context);
static void StdCacheStats(MemoryContext context, int level);
#ifdef MEMORY_CONTEXT_CHECKING
static void StdCacheCheck(MemoryContext context);
#endif

static bool IsInStdPortalCache(StdPortalCache *STDCache,  char *lexicon, char *grammar);
static STANDARDIZER *GetStdFromPortalCache(StdPortalCache *STDCache,  char *lexicon, char *grammar);
static void AddToStdPortalCache(StdPortalCache *STDCache, char *lexicon, char *grammar);
static StdPortalCache *GetStdPortalCache(FunctionCallInfo fcinfo);


/* standardizer api functions */

static STANDARDIZER *CreateStd(char *lexicon, char *grammar);


/* Memory context definition must match the current version of PostgreSQL */
static MemoryContextMethods StdCacheContextMethods =
{
    NULL,
    NULL,
    NULL,
    StdCacheInit,
    StdCacheReset,
    StdCacheDelete,
    NULL,
    StdCacheIsEmpty,
    StdCacheStats
#ifdef MEMORY_CONTEXT_CHECKING
    , StdCacheCheck
#endif
};


static void
StdCacheInit(MemoryContext context)
{
    /* NOP - initialized when first used. */
}


static void
StdCacheReset(MemoryContext context)
{
    // NOP - Seems to be a required function
}


static void
std_free(STANDARDIZER *std)
{
    DBG("Enter: std_free()");
    if (std) {
        if (std->lex_obj) freeLexiconPtr( std->lex_obj );
        std->lex_obj = NULL;
        if (std->gmr_obj) freeGrammarPtr( std->gmr_obj );
        std->gmr_obj = NULL;
        free( std );
    }
    DBG("Leaving: std_free()");
}


static void
StdCacheDelete(MemoryContext context)
{
    StdHashEntry *she;

    DBG("Enter: StdCacheDelete");
    /* lookup the hash entry in the global hash table
       so we can free it */
    she = GetStdHashEntry(context);

    if (!she)
        elog(ERROR, "StdCacheDelete: Trying to delete non-existant hash entry object with MemoryContext key (%p)", (void *)context);

    DBG("deleting std object (%p) with MemoryContext key (%p)", she->std, context);

    if (she->std)
        std_free(she->std);

    DeleteStdHashEntry(context);
}


static bool
StdCacheIsEmpty(MemoryContext context)
{
    // always return false - another required function
    return FALSE;
}


static void
StdCacheStats(MemoryContext context, int level)
{
    // another required function
    fprintf(stderr, "%s: STANDARDIZER context\n", context->name);
}


#ifdef MEMORY_CONTEXT_CHECKING
static void
StdCacheCheck(MemoryContext context)
{
    // NOP - another reuired function
}
#endif


uint32
mcxt_ptr_hash_std(const void *key, Size keysize)
{
    uint32 hashval;
    hashval = DatumGetUInt32(hash_any(key, (int32) keysize));
    return hashval;
}


static void
CreateStdHash(void)
{
    HASHCTL ctl;
    
    ctl.keysize = sizeof(MemoryContext);
    ctl.entrysize = sizeof(StdHashEntry);
    ctl.hash = mcxt_ptr_hash_std;

    StdHash = hash_create("Address Standardizer 2 Backend MemoryContext Hash", STD_BACKEND_HASH_SIZE, &ctl, (HASH_ELEM | HASH_FUNCTION));
    DBG("CreateStdHash: created StdHash (%p)", StdHash);
}


static void
AddStdHashEntry(MemoryContext mcxt, STANDARDIZER *std)
{
    bool found;
    void **key;
    StdHashEntry *he;

    DBG("Enter: AddStdHashEntry(mcxt=%p, std=%p)", mcxt, std);
    /* The hash key is the MemoryContext pointer */
    key = (void *)&mcxt;

    he = (StdHashEntry *) hash_search(StdHash, key, HASH_ENTER, &found);
    DBG("AddStdHashEntry: he=%p, found=%d", he, found);
    if (!found) {
        he->context = mcxt;
        he->std = std;
        DBG("Leaving AddStdHashEntry - Added");
    }
    else {
        elog(ERROR, "AddStdHashEntry: This memory context is already in use! (%p)", (void *)mcxt);
    }
}

static StdHashEntry *
GetStdHashEntry(MemoryContext mcxt)
{
    void **key;
    StdHashEntry *he;

    DBG("Enter: GetStdHashEntry");
    key = (void *)&mcxt;
    he = (StdHashEntry *) hash_search(StdHash, key, HASH_FIND, NULL);
    return he;
}


static void
DeleteStdHashEntry(MemoryContext mcxt)
{
    void **key;
    StdHashEntry *he;

    DBG("Enter: DeleteStdHashEntry");
    key = (void *)&mcxt;
    he = (StdHashEntry *) hash_search(StdHash, key, HASH_REMOVE, NULL);
    if (!he)
        elog(ERROR, "DeleteStdHashEntry: There was an error removing the STD object from this MemoryContext (%p)", (void *)mcxt);

    he->std = NULL;
}


/* public api */
bool
IsInStdCache(StdCache STDCache, char *lexicon, char *grammar) {
    return IsInStdPortalCache((StdPortalCache *) STDCache, lexicon, grammar);
}


static bool
IsInStdPortalCache(StdPortalCache *STDCache,  char *lexicon, char *grammar)
{
    int i;
    bool ret = FALSE;

    DBG("Enter: IsInStdPortalCache");

    char *lex_md5 = getMd5( lexicon );
    DBG("lex_md5: %p", lex_md5);

    char *gmr_md5 = getMd5( grammar );
    DBG("gmr_md5: %p", gmr_md5);
    if ( gmr_md5 == NULL )
        gmr_md5 = strdup("");
    DBG("gmr_md5: %p", gmr_md5);

    DBG("Looping through %d STD_CACHE_ITEMS", STD_CACHE_ITEMS);
    for (i=0; i<STD_CACHE_ITEMS; i++) {
        StdCacheItem *ci = &STDCache->StdCache[i];
        DBG("StdCacheItem[%d] = %p", i, ci);
        DBG("   ci->lex_md5: %p, ci->gmr_md5: %p", ci->lex_md5, ci->gmr_md5);
        if ( ci->lex_md5 && !strcmp(ci->lex_md5, lex_md5 ) &&
             ci->gmr_md5 && !strcmp(ci->gmr_md5, gmr_md5 ) ) {
                ret = TRUE;
                break;
        }
    }
    DBG("End of loop, ret: %d", ret);

    DBG("free( lex_md5 )");
    free( lex_md5 );
    DBG("free( lex_md5 )");
    free( gmr_md5 );

    DBG("Leaving IsInStdPortalCache, ret: %d", ret);
    return ret;
}


/* public api */
STANDARDIZER *
GetStdFromStdCache(StdCache STDCache,  char *lexicon, char *grammar) {
    return GetStdFromPortalCache((StdPortalCache *) STDCache, lexicon, grammar);
}


static STANDARDIZER *
GetStdFromPortalCache(StdPortalCache *STDCache,  char *lexicon, char *grammar)
{
    int i;
    STANDARDIZER *ret = NULL;

    DBG("Enter: GetStdFromPortalCache");

    char *lex_md5 = getMd5( lexicon );
    char *gmr_md5 = getMd5( grammar );
    if ( gmr_md5 == NULL )
        gmr_md5 = strdup("");

    for (i=0; i<STD_CACHE_ITEMS; i++) {
        StdCacheItem *ci = &STDCache->StdCache[i];
        if ( ci->lex_md5 && !strcmp(ci->lex_md5, lex_md5 ) &&
             ci->gmr_md5 && !strcmp(ci->gmr_md5, gmr_md5 ) ) {
                ret = STDCache->StdCache[i].std;
                break;
        }
    }

    free( lex_md5 );
    free( gmr_md5 );

    DBG("GetStdFromPortalCache returning: %p", (void*) ret);

    return ret;
}


static void
DeleteNextSlotFromStdCache(StdPortalCache *STDCache)
{
    MemoryContext old_context;

    DBG("Enter: DeleteNextSlotFromStdCache");
    if (STDCache->StdCache[STDCache->NextSlot].std != NULL) {
        StdCacheItem *ce = &STDCache->StdCache[STDCache->NextSlot];
        DBG("Removing STD cache entry ('%s', '%s') index %d", ce->lex_md5, ce->gmr_md5, STDCache->NextSlot);

        /*
           zero out the entries and free the memory context
           We will get a callback to free the std object.
        */
        old_context = MemoryContextSwitchTo(STDCache->StdCacheContext);
        MemoryContextDelete(ce->std_mcxt);
        pfree(ce->lex_md5);
        ce->lex_md5 = NULL;
        pfree(ce->gmr_md5);
        ce->gmr_md5 = NULL;
        ce->std    = NULL;
        MemoryContextSwitchTo(old_context);
    }
}


/* public api */
void
AddToStdCache(StdCache cache, char *lexicon, char *grammar) {
    AddToStdPortalCache((StdPortalCache *) cache, lexicon, grammar);
}


static void
AddToStdPortalCache(StdPortalCache *STDCache, char *lexicon, char * grammar)
{
    MemoryContext STDMemoryContext;
    MemoryContext old_context;
    STANDARDIZER *std = NULL;
    char * lex_md5;
    char * gmr_md5;

    DBG("Enter: AddToStdPortalCache");

    /* Create the std object */
    std = CreateStd( lexicon, grammar );
    if (!std)
        elog(ERROR,
            "AddToStdPortalCache: could not create address standardizer");

    DBG("Calling getMd5( lexicon )");
    lex_md5 = getMd5( lexicon );
    DBG("Calling getMd5( grammar )");
    gmr_md5 = getMd5( grammar );
    if ( gmr_md5 == NULL )
        gmr_md5 = strdup("");

    /* if the NextSlot in the cache is used, then delete it */
    if (STDCache->StdCache[STDCache->NextSlot].std != NULL) {
#ifdef DEBUG
        StdCacheItem *ce = &STDCache->StdCache[STDCache->NextSlot];
        DBG("Removing item from STD cache ('%s', '%s') index %d", ce->lex_md5, ce->gmr_md5, STDCache->NextSlot);
#endif
        DeleteNextSlotFromStdCache(STDCache);
    }

    DBG("Adding item to STD cache ('%s', '%s') index %d", lex_md5, gmr_md5, STDCache->NextSlot);

    STDMemoryContext = MemoryContextCreate(T_AllocSetContext, 8192,
                                           &StdCacheContextMethods,
                                           STDCache->StdCacheContext,
                                           "AddressStandardizer2 STD Memory Context");

    /* Create the backend hash if it doesn't already exist */
    DBG("Check if StdHash exists (%p)", (void*) StdHash);
    if (!StdHash)
        CreateStdHash();

    /*
     * Add the MemoryContext to the backend hash so we can
     * clean up upon portal shutdown
     */
    DBG("Adding standardizer obj (%p) to hash table with MemoryContext key (%p)", (void*) std, (void*) STDMemoryContext);

    AddStdHashEntry(STDMemoryContext, std);

    /* change memory contexts so the pstrdup are allocated in the
     * context of this cache item. They will be freed when the
     * cache item is deleted.
    */
    DBG("AddToStdPortalCache: changing memory context to %p", STDCache->StdCacheContext);
    old_context = MemoryContextSwitchTo(STDCache->StdCacheContext);
    DBG("  old_context= %p", (void*) old_context);
    STDCache->StdCache[STDCache->NextSlot].lex_md5 = pstrdup(lex_md5);
    free( lex_md5 );
    DBG("  pstrdup(lex_md5) completed");
    STDCache->StdCache[STDCache->NextSlot].gmr_md5 = pstrdup(gmr_md5);
    free( gmr_md5 );
    DBG("  pstrdup(gmr_md5) completed");
    MemoryContextSwitchTo(old_context);
    DBG(" changed memory context to %p", (void*) old_context);

    STDCache->StdCache[STDCache->NextSlot].std = std;
    STDCache->StdCache[STDCache->NextSlot].std_mcxt = STDMemoryContext;
    STDCache->NextSlot = (STDCache->NextSlot + 1) % STD_CACHE_ITEMS;
    DBG("STDCache->NextSlot=%d", STDCache->NextSlot);
}


/* pubilc api */
StdCache
GetStdCache(FunctionCallInfo fcinfo) {
    return (StdCache) GetStdPortalCache(fcinfo);
}


static StdPortalCache *
GetStdPortalCache(FunctionCallInfo fcinfo)
{
    StdPortalCache *STDCache;

    DBG("Enter: GetStdPortalCache");
    /* create it if we don't already have one for this portal */
    if (fcinfo->flinfo->fn_extra == NULL) {
        MemoryContext old_context;

        old_context = MemoryContextSwitchTo(fcinfo->flinfo->fn_mcxt);
        STDCache = palloc(sizeof(StdPortalCache));
        MemoryContextSwitchTo(old_context);

        if (STDCache) {
            int i;

            DBG("Allocating STDCache for portal with STD MemoryContext (%p)", (void*) fcinfo->flinfo->fn_mcxt);
            /* initial the cache items */
            for (i=0; i<STD_CACHE_ITEMS; i++) {
                STDCache->StdCache[i].lex_md5  = NULL;
                STDCache->StdCache[i].gmr_md5  = NULL;
                STDCache->StdCache[i].std      = NULL;
                STDCache->StdCache[i].std_mcxt = NULL;
            }
            STDCache->NextSlot = 0;
            STDCache->StdCacheContext = fcinfo->flinfo->fn_mcxt;

            /* Store the pointer in fcinfo->flinfo->fn_extra */
            fcinfo->flinfo->fn_extra = STDCache;
        }
    }
    else {
        /* Use the existing cache */
        STDCache = fcinfo->flinfo->fn_extra;
    }

    DBG("GetStdPortalCache: Using cache entry at (%p) in mcxt (%p)", (void*) STDCache, (void*)STDCache->StdCacheContext);

    return STDCache;
}

/* public api */
STANDARDIZER *
GetStdUsingFCInfo(FunctionCallInfo fcinfo, char *lexicon, char *grammar)
{
    STANDARDIZER *std;
    StdCache *std_cache = NULL;

    DBG("GetStdUsingFCInfo: calling GetStdCache(fcinfo)");
    std_cache = GetStdCache(fcinfo);
    DBG("GetStdUsingFCInfo: got: (%p)", (void*)std_cache);
    if (!std_cache)
        return NULL;

    DBG("GetStdUsingFCInfo: calling IsInStdCache(std_cache, lexicon, grammar)");
    if (!IsInStdCache(std_cache, lexicon, grammar)) {
        DBG("GetStdUsingFCInfo: calling AddToStdCache(std_cache, lexicon, grammar)");
        AddToStdCache(std_cache, lexicon, grammar);
    }

    DBG("GetStdUsingFCInfo: GetStdFromStdCache(std_cache, lexicon, grammar)");
    std = GetStdFromStdCache(std_cache, lexicon, grammar);
    DBG("GetStdUsingFCInfo: std=%p", std);

    return std;
}


static STANDARDIZER *
CreateStd(char *lexicon, char *grammar)
{
    STANDARDIZER     *std;
    void             *lex;
    void             *gmr;
    char            **err_msg = NULL;
    char             *pmsg;

    DBG("Enter: CreateStd");

    std = (STANDARDIZER *) calloc(1, sizeof(STANDARDIZER));
    if (!std)
        elog(ERROR, "CreateStd: could not allocate memory (std)");
    DBG("CreateStd: std=%p", (void*)std);


    DBG("Calling getLexiconPtr( lexicon, err_msg )");
    lex = getLexiconPtr( lexicon, err_msg );
    DBG("Back from (%p) getLexiconPtr( lexicon, err_msg )", lex);
    if (!lex) {
        std_free(std);
        SPI_finish();
        pmsg = pstrdup(*err_msg);
        free(*err_msg);
        elog(ERROR, "CreateStd: failed to load Lexicon (%s)", pmsg);
    }
    std->lex_obj = lex;

    DBG("Calling getGrammarPtr( grammar, err_msg )");
    gmr = getGrammarPtr( grammar, err_msg );
    DBG("Back from (%p) getGrammarPtr( grammar, err_msg )", gmr);
    if (!gmr) {
        freeLexiconPtr( lex );
        std_free(std);
        SPI_finish();
        pmsg = pstrdup(*err_msg);
        free(*err_msg);
        elog(ERROR, "CreateStd: failed to load Gammar (%s)", pmsg);
    }
    std->gmr_obj = gmr;

#ifdef DEBUG
    {
    char             *md5hash;
    DBG("Fetching md5 hashes");
    md5hash = getLexiconMd5( lex );
    DBG("Lexicon md5: %s", md5hash);
    free(md5hash);
    md5hash = getGrammarMd5( gmr );
    DBG("Grammar md5: %s", md5hash);
    free(md5hash);
    }
#endif


    DBG("Returning from CreateStd");
    return std;
}


