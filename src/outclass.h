/*HEADER*
*HEADER*/
#ifndef OUTCLASS_H
#define OUTCLASS_H

#include <map>
#include <string>

class OutClass {
public:

    typedef enum {
        STOP    = -1,   ///< End of string of tokens in rules.
        BLDNG   =  0,   ///< Unparsed building identifiers and types.
        HOUSE   =  1,   ///< The civic address number. The '123' in '123 MAIN ST'
        PREDIR  =  2,   ///< A directional modifier that precedes the street name. Example, the 'WEST' in '3715 TENTH AVENUE WEST'.
        QUALIF  =  3,   ///< A pre or post modifier. Example, the 'OLD' in '3715 OLD HIGHWAY 99' or 'BUSINESS' in '1203 HIGHWAY 45 BUSINESS'.
        PRETYP  =  4,   ///< A street type preceding the root street name. Example, the 'HIGHWAY' in '3715 HIGHWAY 99'.
        STREET  =  5,   ///< The root street name, stripped of directional or type modifiers. Example, the 'TENTH' in '3715 WEST TENTH AVENUE'.
        SUFTYP  =  6,   ///< A street type following the root street name. Example, the 'AVENUE' in '3715 WEST TENTH AVENUE'.
        SUFDIR  =  7,   ///< A directional modifier that follows the street name. Example, the 'WEST' in '3715 TENTH AVENUE WEST'.
        RR      =  8,   ///< The 'RR' in 'RR 7'
        EXTRA   =  9,   ///< An otherwise unclassified output.
        CITY    = 10,   ///< A placename. Example, 'LONDON'
        PROV    = 11,   ///< A state, province or region. Example, 'NY' or 'NEW YORK'.
        NATION  = 12,   ///< A country name or abbreviation.
        POSTAL  = 13,   ///< A postal code component.
        BOXH    = 14,   ///< The 'BOX' in 'BOX 3B'
        BOXT    = 15,   ///< The '3B' in 'BOX 3B'
        UNITH   = 16,   ///< The 'APT' in 'APT 3B'
        UNITT   = 17,   ///< The '3B' in 'APT 3B'
        BADTOKEN = 99   ///< An bad token type
    } Type;

    static std::string asString(const OutClass::Type &t);
    static OutClass::Type asType(const std::string &s);
    static OutClass::Type asType(const int t);

};

#endif
