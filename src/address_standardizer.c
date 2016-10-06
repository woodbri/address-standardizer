/**ADDRESS_STANDARDIZER***************************************************
 *
 * Address Standardizer
 *      A collection of C++ classes for parsing street addresses
 *      and standardizing them for the purpose of Geocoding.
 *
 * Copyright 2016 Stephen Woodbridge <woodbri@imaptools.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file LICENSE for details.
 *
 ***************************************************ADDRESS_STANDARDIZER**/

#include "postgres.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#include "utils/builtins.h"

#define USE_QUERY_CACHE

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#include <stdio.h>
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif


#include "std_pg_hash.h"
#include "address_standardizer.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PGDLLEXPORT Datum as_compile_lexicon(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum as_standardize(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum as_parse(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum as_match(PG_FUNCTION_ARGS);


void stdaddr_free(STDADDR *stdaddr);
static char *text2char(text *in);

static char *text2char(text *in)
{
    char *out = palloc(VARSIZE(in));
    memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
    out[VARSIZE(in) - VARHDRSZ] = '\0';
    return out;
}


void stdaddr_free(STDADDR *stdaddr)
{
    if (!stdaddr) return;
    if (stdaddr->building)   free(stdaddr->building);
    if (stdaddr->house_num)  free(stdaddr->house_num);
    if (stdaddr->predir)     free(stdaddr->predir);
    if (stdaddr->qual)       free(stdaddr->qual);
    if (stdaddr->pretype)    free(stdaddr->pretype);
    if (stdaddr->name)       free(stdaddr->name);
    if (stdaddr->suftype)    free(stdaddr->suftype);
    if (stdaddr->sufdir)     free(stdaddr->sufdir);
    if (stdaddr->ruralroute) free(stdaddr->ruralroute);
    if (stdaddr->extra)      free(stdaddr->extra);
    if (stdaddr->city)       free(stdaddr->city);
    if (stdaddr->prov)       free(stdaddr->prov);
    if (stdaddr->country)    free(stdaddr->country);
    if (stdaddr->postcode)   free(stdaddr->postcode);
    if (stdaddr->box)        free(stdaddr->box);
    if (stdaddr->unit)       free(stdaddr->unit);
    free(stdaddr);
    stdaddr = NULL;
}


/*
 *  CREATE OR REPLACE FUNCTION as_compile_lexicon(
 *          lexicon text
 *          )
 *      RETURNS TEXT
 *      AS '$libdir/address_standardizer-2.0', 'compile_lexicon'
 *      LANGUAGE 'c' IMMUTABLE STRICT;
 *
*/
PG_FUNCTION_INFO_V1(as_compile_lexicon);

Datum as_compile_lexicon(PG_FUNCTION_ARGS)
{
    char *lexicon;
    char *cstr = NULL;
    char *err_msg = NULL;
    char *pstr;

    lexicon = text2char(PG_GETARG_TEXT_P(0));
    DBG("lexicon:\n '%s'", lexicon);

    cstr = serialize_lexicon( lexicon, &err_msg );
    if ( err_msg != NULL )
        elog(ERROR, "std_standardize threw an error: %s", err_msg);

    if (cstr) {
        pstr = pstrdup(cstr);
        free(cstr);
    }
    else
        elog(ERROR, "lexicon failed to compile!");


    PG_RETURN_TEXT_P(cstring_to_text(pstr));
}


/*
 *  CREATE OR REPLACE FUNCTION as_standardize(
 *          address text,
 *          grammar text,
 *          lexicon text,
 *          locale text,
 *          filter text DEFAULT 'PUNCT,SPACE,DASH,EMDASH',
 *          OUT building text,
 *          OUT house_num text,
 *          OUT predir text,
 *          OUT qual text,
 *          OUT pretype text,
 *          OUT name text,
 *          OUT suftype text,
 *          OUT sufdir text,
 *          OUT ruralroute text,
 *          OUT extra text,
 *          OUT city text,
 *          OUT prov text,
 *          OUT country text,
 *          OUT postcode text,
 *          OUT box text,
 *          OUT unit text
 *          )
 *      RETURNS RECORD
 *      AS '$libdir/address_standardizer-2.0', 'standardize_address'
 *      LANGUAGE 'c' IMMUTABLE STRICT;
 *
 * If you want to standardize a whole table then call it like:
 *
 *   insert into stdaddr (...)
 *       select id, (std).* from (
 *           select as_standardize(
 *               address, grammar, lexicon, config) as std
 *             from table_to_standardize) as foo;
 *
 *
*/

PG_FUNCTION_INFO_V1(as_standardize);

Datum as_standardize(PG_FUNCTION_ARGS)
{
    TupleDesc            tuple_desc;
    AttInMetadata       *attinmeta;
    char                *address;
    char                *grammar;
    char                *lexicon;
    char                *locale;
    char                *filter;
    Datum                result;
    STDADDR             *stdaddr;
    char               **values;
    HeapTuple            tuple;
    int                  k;
    STANDARDIZER        *std;
    char                *err_msg; 

    DBG("Start as_standardize");

    address = text2char(PG_GETARG_TEXT_P(0));
    DBG("address: '%s'", address);
    grammar = text2char(PG_GETARG_TEXT_P(1));
    DBG("grammar:\n '%s'", grammar);
    lexicon = text2char(PG_GETARG_TEXT_P(2));
    DBG("lexicon:\n '%s'", lexicon);
    locale  = text2char(PG_GETARG_TEXT_P(3));
    DBG("locale: '%s'", locale);
    filter  = text2char(PG_GETARG_TEXT_P(4));
    DBG("filter: '%s'", filter);

    DBG("calling RelationNameGetTupleDesc");
    if (get_call_result_type( fcinfo, NULL, &tuple_desc ) != TYPEFUNC_COMPOSITE ) {
        elog(ERROR, "as_standardize() was called in a way that cannot accept record as a result");
    }
    BlessTupleDesc(tuple_desc);
    attinmeta = TupleDescGetAttInMetadata(tuple_desc);

/* 
   The following code is code be used to implement a query level cache
   of the standardizer, so it does not have to be initialized  for every
   address. Else we will take the slower approach.
*/
#ifdef USE_QUERY_CACHE
    std = GetStdUsingFCInfo( fcinfo, lexicon, grammar );
    if (!std)
        elog(ERROR, "as_standardize() failed to create the address standardizer object!");

    DBG("calling std_standardize('%s')", address);
    stdaddr = std_standardize_ptrs( address, std->gmr_obj, std->lex_obj, locale, filter, &err_msg );
#else
    DBG("calling std_standardize('%s')", address);
    stdaddr = std_standardize(address, grammar, lexicon, locale, filter, &err_msg);
#endif
    DBG("back from std_standardize");

    if ( err_msg != NULL )
        DBG("std_standardize threw an error: %s", err_msg);

    values = (char **) palloc(16 * sizeof(char *));
    for (k=0; k<16; k++) {
        values[k] = NULL;
    }
    DBG("setup values array for natts=%d", tuple_desc->natts);
    if (stdaddr) {
        values[0] = stdaddr->building   ? pstrdup(stdaddr->building) : NULL;
        values[1] = stdaddr->house_num  ? pstrdup(stdaddr->house_num) : NULL;
        values[2] = stdaddr->predir     ? pstrdup(stdaddr->predir) : NULL;
        values[3] = stdaddr->qual       ? pstrdup(stdaddr->qual) : NULL;
        values[4] = stdaddr->pretype    ? pstrdup(stdaddr->pretype) : NULL;
        values[5] = stdaddr->name       ? pstrdup(stdaddr->name) : NULL;
        values[6] = stdaddr->suftype    ? pstrdup(stdaddr->suftype) : NULL;
        values[7] = stdaddr->sufdir     ? pstrdup(stdaddr->sufdir) : NULL;
        values[8] = stdaddr->ruralroute ? pstrdup(stdaddr->ruralroute) : NULL;
        values[9] = stdaddr->extra      ? pstrdup(stdaddr->extra) : NULL;
        values[10] = stdaddr->city      ? pstrdup(stdaddr->city) : NULL;
        values[11] = stdaddr->prov      ? pstrdup(stdaddr->prov) : NULL;
        values[12] = stdaddr->country   ? pstrdup(stdaddr->country) : NULL;
        values[13] = stdaddr->postcode  ? pstrdup(stdaddr->postcode) : NULL;
        values[14] = stdaddr->box       ? pstrdup(stdaddr->box) : NULL;
        values[15] = stdaddr->unit      ? pstrdup(stdaddr->unit) : NULL;
    }

    DBG("calling heap_form_tuple");
    tuple = BuildTupleFromCStrings(attinmeta, values);

    /* make the tuple into a datum */
    DBG("calling HeapTupleGetDatum");
    result = HeapTupleGetDatum(tuple);

    /* clean up (this is not really necessary */
    DBG("freeing values, nulls, and stdaddr");
    stdaddr_free(stdaddr);

    DBG("returning standardized result");
    PG_RETURN_DATUM(result);
}



/*
 *  CREATE OR REPLACE FUNCTION as_parse(
 *          address text,
 *          lexicon text,
 *          locale text,
 *          filter text,
 *          OUT seq integer,
 *          OUT word text,
 *          OUT stdword text,
 *          OUT inclass text,
 *          OUT outclass text,
 *          OUT attached text
 *          )
 *      RETURNS SETOF RECORD
 *      AS '$libdir/address_standardizer-2.0', 'parse'
 *      LANGUAGE 'c' IMMUTABLE STRICT;
 *
 *
*/

PG_FUNCTION_INFO_V1(as_parse);

Datum as_parse(PG_FUNCTION_ARGS)
{
    char                *address;
    char                *lexicon;
    char                *locale;
    char                *filter;


    DBG("Start as_parse");

    address = text2char(PG_GETARG_TEXT_P(0));
    lexicon = text2char(PG_GETARG_TEXT_P(1));
    locale  = text2char(PG_GETARG_TEXT_P(2));
    filter  = text2char(PG_GETARG_TEXT_P(3));

    elog(ERROR, "as_parse() not implemented yet!.");
}


/*
 *  CREATE OR REPLACE FUNCTION as_match(
 *          tokens text,
 *          grammar text,
 *          node text default 'ADDRESS',
 *          OUT id bigint,
 *          OUT seq integer,
 *          OUT word text,
 *          OUT stdword text,
 *          OUT inclass text,
 *          OUT outclass text,
 *          OUT attached text
 *          )
 *      RETURNS SETOF RECORD
 *      AS '$libdir/address_standardizer-2.0', 'parse'
 *      LANGUAGE 'c' IMMUTABLE STRICT;
*/

PG_FUNCTION_INFO_V1(as_match);

Datum as_match(PG_FUNCTION_ARGS)
{
    char                *tokens;
    char                *grammar;
    char                *node;


    DBG("Start as_match");

    tokens  = text2char(PG_GETARG_TEXT_P(0));
    grammar = text2char(PG_GETARG_TEXT_P(1));
    node    = text2char(PG_GETARG_TEXT_P(2));

    elog(ERROR, "as_match() not implemented yet!.");
}


