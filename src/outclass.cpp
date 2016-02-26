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

#include "outclass.h"

std::string OutClass::asString(const OutClass::Type &t) {
    std::string str;
    switch (t) {
        case STOP:      str = "STOP";       break;
        case BLDNG:     str = "BLDNG";      break;
        case HOUSE:     str = "HOUSE";      break;
        case PREDIR:    str = "PREDIR";     break;
        case QUALIF:    str = "QUALIF";     break;
        case PRETYP:    str = "PRETYP";     break;
        case STREET:    str = "STREET";     break;
        case SUFTYP:    str = "SUFTYP";     break;
        case SUFDIR:    str = "SUFDIR";     break;
        case RR:        str = "RR";         break;
        case EXTRA:     str = "EXTRA";      break;
        case CITY:      str = "CITY";       break;
        case PROV:      str = "PROV";       break;
        case NATION:    str = "NATION";     break;
        case POSTAL:    str = "POSTAL";     break;
        case BOXH:      str = "BOXH";       break;
        case BOXT:      str = "BOXT";       break;
        case UNITH:     str = "UNITH";      break;
        case UNITT:     str = "UNITT";      break;
        case IGNORE:    str = "IGNORE";     break;
        case BADTOKEN:  str = "BADTOKEN";   break;
    };
    return str;
}


OutClass::Type OutClass::asType(const std::string &s) {
    OutClass::Type t;
    std::map<std::string, OutClass::Type> m;
    std::map<std::string, OutClass::Type>::iterator it;

    m["STOP"]      = STOP;
    m["BLDNG"]     = BLDNG;
    m["HOUSE"]     = HOUSE;
    m["PREDIR"]    = PREDIR;
    m["QUALIF"]    = QUALIF;
    m["PRETYP"]    = PRETYP;
    m["STREET"]    = STREET;
    m["SUFTYP"]    = SUFTYP;
    m["SUFDIR"]    = SUFDIR;
    m["RR"]        = RR;
    m["EXTRA"]     = EXTRA;
    m["CITY"]      = CITY;
    m["PROV"]      = PROV;
    m["NATION"]    = NATION;
    m["POSTAL"]    = POSTAL;
    m["BOXH"]      = BOXH;
    m["BOXT"]      = BOXT;
    m["UNITH"]     = UNITH;
    m["UNITT"]     = UNITT;
    m["IGNORE"]    = IGNORE;
    m["BADTOKEN"]  = BADTOKEN;

    it = m.find(s);
    if (it == m.end())
        t = BADTOKEN;
    else
        t = it->second;

    return t;
}


OutClass::Type OutClass::asType(const int i) {
    OutClass::Type t;
    switch (i) {
        case -1: t = STOP;      break;
        case  0: t = BLDNG;     break;
        case  1: t = HOUSE;     break;
        case  2: t = PREDIR;    break;
        case  3: t = QUALIF;    break;
        case  4: t = PRETYP;    break;
        case  5: t = STREET;    break;
        case  6: t = SUFTYP;    break;
        case  7: t = SUFDIR;    break;
        case  8: t = RR;        break;
        case  9: t = EXTRA;     break;
        case 10: t = CITY;      break;
        case 11: t = PROV;      break;
        case 12: t = NATION;    break;
        case 13: t = POSTAL;    break;
        case 14: t = BOXH;      break;
        case 15: t = BOXT;      break;
        case 16: t = UNITH;     break;
        case 17: t = UNITT;     break;
        case 98: t = IGNORE;    break;
        default: t = BADTOKEN;  break;
    }
    return t;
}

