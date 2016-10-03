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

#ifndef ADDRESS_STANDARDIZER_H
#define ADDRESS_STANDARDIZER_H


typedef struct {
   char *building;
   char *house_num;
   char *predir;
   char *qual;
   char *pretype;
   char *name;
   char *suftype;
   char *sufdir;
   char *ruralroute;
   char *extra;
   char *city;
   char *prov;
   char *country;
   char *postcode;
   char *box;
   char *unit;
} STDADDR;


#ifdef __cplusplus
extern "C"
#endif

STDADDR *std_standardize(
    char *address_in,
    char *grammar_in,
    char *lexicon_in,
    char *locale_in,
    char *filter_in,
    char **err_msg
);


#ifdef __cplusplus
extern "C"
#endif
void *getGrammarPtr( char *grammar_in );

#ifdef __cplusplus
extern "C"
#endif
void freeGrammarPtr( void *ptr );

#ifdef __cplusplus
extern "C"
#endif
void *getLexiconPtr( char *lexicon_in );

#ifdef __cplusplus
extern "C"
#endif
void freeLexiconPtr( void *ptr );

#ifdef __cplusplus
extern "C"
#endif
char *getGrammarMd5( void *ptr );

#ifdef __cplusplus
extern "C"
#endif
char *getLexiconMd5( void *ptr );

#ifdef __cplusplus
extern "C"
#endif
char *getMd5( char *text );

char * serialize_lexicon(
    char *lexicon_in,
    char **err_msg
);


#endif
