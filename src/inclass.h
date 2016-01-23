/*HEADER*
*HEADER*/
#ifndef INCLASS_H
#define INCLASS_H

#include <map>
#include <set>
#include <string>

class InClass {
public:

    typedef enum {
        STOP    = -1,   ///< undefined token
        NUMBER  =  0,   ///< A string of digits.
        WORD    =  1,   ///< A word is a string of letters of arbitrary length. A single letter can be both a SINGLE and a WORD.
        TYPE    =  2,   ///< Words and abbreviation used to denote street typess. For example, 'ST' or 'AVE'.
        QUALIF  =  3,   ///< Words that qualify a street name, like, 'OLD', 'EXTENSION', 'BUSINESS', 'ALTERNATE', etc.
        ROAD    =  6,   ///< Words and abbreviations used to denote highways and roads. Example: the 'Interstate' in 'Interstate 5'.
        STOPWORD = 7,   ///< A word with low lexical significance, that can be omitted in parsing. For example, 'THE'. STOPWORDS combine with WORDS. In rules a string of multiple WORDs and STOPWORDs will be represented by a single WORD token.
        RR      =  8,   ///< Words and abbreviations used to denote rural routes. 'RR'.
        DASH    =  9,   ///< A hyphen
        CITY    = 10,   ///< City name
        PROV    = 11,   ///< State, province, or region name or abbreviation
        NATION  = 12,   ///< Nation name or abbreviation
        AMPERS  = 13,   ///< The ampersand (&) is frequently used to abbreviate the word 'and'.
        BOXH    = 14,   ///< Words used to denote post office boxes. For example 'Box' or 'PO Box'.
        ORD     = 15,   ///< Representations such as First or 1st. Often used in street. Are often standardized as numbers.
        UNITH   = 16,   ///< Words and abbreviation used to denote internal subaddresses. For example, 'APT' or 'UNIT'.
        SINGLE  = 18,   ///< A single letter.
        BUILDH  = 19,   ///< Words used to denote buildings or building complexes, usually as a prefix. For example 'Tower' in 'Tower 7A'.
        MILE    = 20,   ///< Words used to denote milepost addresses.
        DOUBLE  = 21,   ///< A sequence of two letters. Often used as identifiers.
        DIRECT  = 22,   ///< Words used to denote directions, for example 'North'. These should get standardized to the full word not the abreviation.
        MIXED   = 23,   ///< An alphanumeric string that contains both letters and digits. Used for identifiers.
        BUILDT  = 24,   ///< Words and abbreviations used to denote buildings or building complexes, usually as a suffix. For example, 'Shopping Centre'.
        FRACT   = 25,   ///< Fractions are sometimes used in civic numbers or unit numbers.
        PCT     = 26,   ///< A 3 character sequence of number letter number. Identifies an LDU, the last 3 characters of a Canadian postal code.
        PCH     = 27,   ///< A 3 character sequence of letter number letter. Identifies an FSA, the first 3 characters of a Canadian postal code.
        QUINT   = 28,   ///< A 5 digit number. Identifies a Zip Code
        QUAD    = 29,   ///< A 4 digit number. Identifies ZIP4.
        PUNCT   = 30,   ///< Punctuation character(s)
        SPACE   = 31,   ///< whitespace
        PLACEN  = 32,   ///< Placename keyword
        EMDASH  = 33,   ///< Token used to identify where a word was split by the tokenizer
        BADTOKEN = 99   ///< A bad token type.
    } Type;

    typedef enum {
        ALB =0, ///< ALBANIAN
        ARA,    ///< ARABIC
        BAQ,    ///< BASQUE
        BEL,    ///< BELARUSIAN
        BET,    ///< BELARUSIAN TRANSCRIBED
        BEX,    ///< BELARUSIAN TRANSLITERATED
        BOS,    ///< BOSNIAN
        BOX,    ///< BOSNIAN TRANSLITERATED
        BUL,    ///< BULGARIAN
        BUT,    ///< BULGARIAN TRANSCRIBED
        BUX,    ///< BULGARIAN TRANSLITERATED
        CAT,    ///< CATALAN
        CHI,    ///< CHINESE (MODERN)
        CHT,    ///< CHINESE (TRADITIONAL)
        CZE,    ///< CZECH
        CZX,    ///< CZECH TRANSLITERATED
        DAN,    ///< DANISH
        DUT,    ///< DUTCH
        ENG,    ///< ENGLISH
        EST,    ///< ESTONIAN
        ESX,    ///< ESTONIAN TRANSLITERATED
        FIN,    ///< FINNISH
        FRE,    ///< FRENCH
        GER,    ///< GERMAN
        GLG,    ///< GALICIAN
        GRE,    ///< GREEK
        GRN,    ///< GUARANI
        GRT,    ///< GREEK TRANSCRIBED
        GRX,    ///< GREK TRANSLITERATED
        HEB,    ///< Hebrew
        HUN,    ///< HUNGARIAN
        HUX,    ///< HUNGARIAN TRANSLITERATED
        IND,    ///< BAHASA INDONESIA
        ITA,    ///< ITALIAN
        KOR,    ///< KOREAN
        KOX,    ///< KOREAN TRANSLITERATED
        LAV,    ///< LATVIAN
        LAX,    ///< LATVIAN TRANSLITERATED
        LIT,    ///< LITHUANIAN
        LIX,    ///< LITHUANIAN TRANSLITERATED
        MAC,    ///< MACEDONIAN
        MAT,    ///< MACEDONIAN TRANSCRIBED
        MAY,    ///< MALAYSIAN
        MLT,    ///< MALTESE
        MLX,    ///< MALTESE TRANSLITERATED
        MNE,    ///< MONTENEGRIN
        MNX,    ///< MONTENEGRIN TRANSLITERATED
        MOL,    ///< MOLDOVAN
        MOX,    ///< MOLDOVAN TRANSLITERATED
        NOR,    ///< NORWEGIAN
        POL,    ///< POLISH
        POR,    ///< PORTUGUESE
        POX,    ///< POLISH TRANSLITERATED
        PYN,    ///< PINYIN
        RMX,    ///< ROMANIAN TRANSLITERATED
        RST,    ///< RUSSIAN TRANSCRIBED
        RUM,    ///< ROMANIAN
        RUS,    ///< RUSSIAN
        RUX,    ///< RUSSIAN TRANSLITERATED
        SCR,    ///< CROATIAN
        SCT,    ///< SERBIAN TRANSCRIBED
        SCX,    ///< SERBIAN TRANSLITERATED
        SIX,    ///< SLOVENIAN TRANSLITERATED
        SLO,    ///< SLOVAK
        SLV,    ///< SLOVENIAN
        SLX,    ///< SLOVAK TRANSLITERATED
        SPA,    ///< SPANISH
        SRB,    ///< SERBIAN
        SRX,    ///< CROATION TRANSLITERATED
        SWE,    ///< SWEDISH
        THA,    ///< THAI
        TKT,    ///< TURKISH TRANSCRIBED
        TUR,    ///< TURKISH
        TUX,    ///< TURKISH TRANSLITERATED
        UKR,    ///< UKRAINIAN
        UKT,    ///< UKRAINIAN TRANSCRIBED
        UKX,    ///< UKRAINIAN TRANSLITERATED
        WEL,    ///< WELSH
        UNKNOWN = -1    ///< UNKNOWN or UNDEFINED
    } Lang;

    typedef enum {
        DET_PRE  = 0,   ///< detached prefix
        DET_SUF  = 1,   ///< detached suffix
        ATT_PRE  = 2,   ///< attached prefix
        ATT_SUF  = 3    ///< attached suffix
    } AttachType;


    static std::string asString(const std::set<InClass::Type> &t);
    static std::string asString(const InClass::Type &t);
    static std::set<InClass::Type> asType(const std::string &s);
    static InClass::Type asType(const int i);
    static std::set<InClass::AttachType> asAttachType(const std::string &s);

    static std::string asString(const InClass::Lang &lang);
    static std::string asName(const InClass::Lang &lang);
    static InClass::Lang asLang(const std::string &s);
    static std::string asString(const std::set<InClass::AttachType> &t);
    static std::string asString(const InClass::AttachType &t);

private:

};

#endif
