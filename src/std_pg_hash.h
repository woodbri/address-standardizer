

#include "address_standardizer.h"

/* Opaque type to use in standardizer cache API */
typedef void *StdCache;

StdCache GetStdCache(FunctionCallInfo fcinfo);
bool IsInStdCache(StdCache STDCache, char *lexicon, char *grammar);
void AddToStdCache(StdCache cache, char *lexicon, char *grammar);
STANDARDIZER *GetStdFromStdCache(StdCache STDCache,  char *lexicon, char *grammar);

/*
 * This is the only interface external code should be calling
 * it will get the standardizer out of the cache, or
 * it will create a new one and save it in the cache
*/
STANDARDIZER *GetStdUsingFCInfo(FunctionCallInfo fcinfo, char *lexicon, char *grammar);


