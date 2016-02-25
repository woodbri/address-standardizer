
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


#endif
