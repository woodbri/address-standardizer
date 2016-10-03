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

#include <sstream>
#include <iostream>

#include "inclass.h"

// non-class utility function to trim leading and trailing spaces
inline std::string trimboth( std::string str ) {
    auto start = str.find_first_not_of(" ");
    if ( start == std::string::npos ) start = 0;
    auto end = str.find_last_not_of(" ");
    if ( end == std::string::npos ) end = str.size();
    return std::string( str, start, end - start + 1 );
}


std::string InClass::asString(const InClass::Type &t) {
    std::string str;
    switch (t) {
        case STOP:      str = "STOP";   break;
        case NUMBER:    str = "NUMBER"; break;
        case WORD:      str = "WORD";   break;
        case TYPE:      str = "TYPE";   break;
        case QUALIF:    str = "QUALIF"; break;
        case ROAD:      str = "ROAD";   break;
        case STOPWORD:  str = "STOPWORD"; break;
        case RR:        str = "RR";     break;
        case DASH:      str = "DASH";   break;
        case CITY:      str = "CITY";   break;
        case PROV:      str = "PROV";   break;
        case NATION:    str = "NATION"; break;
        case AMPERS:    str = "AMPERS"; break;
        case BOXH:      str = "BOXH";   break;
        case ORD:       str = "ORD";    break;
        case UNITH:     str = "UNITH";  break;
        case UNITT:     str = "UNITT";  break;
        case SINGLE:    str = "SINGLE"; break;
        case BUILDH:    str = "BUILDH"; break;
        case MILE:      str = "MILE";   break;
        case DOUBLE:    str = "DOUBLE"; break;
        case DIRECT:    str = "DIRECT"; break;
        case MIXED:     str = "MIXED";  break;
        case BUILDT:    str = "BUILDT"; break;
        case FRACT:     str = "FRACT";  break;
        case PCT:       str = "PCT";    break;
        case PCH:       str = "PCH";    break;
        case QUINT:     str = "QUINT";  break;
        case QUAD:      str = "QUAD";   break;
        case PUNCT:     str = "PUNCT";  break;
        case SPACE:     str = "SPACE";  break;
        case PLACEN:    str = "PLACEN"; break;
        case EMDASH:    str = "EMDASH"; break;
        case SLASH:     str = "SLASH";  break;
        case ATSIGN:    str = "ATSIGN"; break;
        case COMMA:     str = "COMMA";  break;
        default:        str = "BADTOKEN"; break;
    };
    return str;
}


InClass::Type InClass::asOneType(const std::string &s) {
    InClass::Type t;

    if      ( s == "STOP" )     t = STOP;
    else if ( s == "NUMBER" )   t = NUMBER;
    else if ( s == "WORD" )     t = WORD;
    else if ( s == "TYPE" )     t = TYPE;
    else if ( s == "QUALIF" )   t = QUALIF;
    else if ( s == "ROAD" )     t = ROAD;
    else if ( s == "STOPWORD" ) t = STOPWORD;
    else if ( s == "RR" )       t = RR;
    else if ( s == "DASH" )     t = DASH;
    else if ( s == "CITY" )     t = CITY;
    else if ( s == "PROV" )     t = PROV;
    else if ( s == "NATION" )   t = NATION;
    else if ( s == "AMPERS" )   t = AMPERS;
    else if ( s == "BOXH" )     t = BOXH;
    else if ( s == "ORD" )      t = ORD;
    else if ( s == "UNITH" )    t = UNITH;
    else if ( s == "UNITT" )    t = UNITT;
    else if ( s == "SINGLE" )   t = SINGLE;
    else if ( s == "BUILDH" )   t = BUILDH;
    else if ( s == "MILE" )     t = MILE;
    else if ( s == "DOUBLE" )   t = DOUBLE;
    else if ( s == "DIRECT" )   t = DIRECT;
    else if ( s == "MIXED" )    t = MIXED;
    else if ( s == "BUILDT" )   t = BUILDT;
    else if ( s == "FRACT" )    t = FRACT;
    else if ( s == "PCT" )      t = PCT;
    else if ( s == "PCH" )      t = PCH;
    else if ( s == "QUINT" )    t = QUINT;
    else if ( s == "QUAD" )     t = QUAD;
    else if ( s == "PUNCT" )    t = PUNCT;
    else if ( s == "SPACE" )    t = SPACE;
    else if ( s == "PLACEN" )   t = PLACEN;
    else if ( s == "EMDASH" )   t = EMDASH;
    else if ( s == "SLASH" )    t = SLASH;
    else if ( s == "ATSIGN" )   t = ATSIGN;
    else if ( s == "COMMA" )    t = COMMA;
    else                        t = BADTOKEN;

    return t;
}


std::set<InClass::Type> InClass::asType(const std::string &s) {
    InClass::Type t;
    std::set<InClass::Type> ret;
    std::stringstream buffer(s);
    std::string word;
    while (true) {
        std::getline(buffer, word, ',');
        word = trimboth( word );
        if (word.length() > 0) {
            t = asOneType( word );
            ret.insert( t );
        }
        if (buffer.eof())
            break;
    }
    return ret;
}


std::string InClass::asString(const std::set<InClass::Type> &t) {
    std::string str;
    for (auto it=t.begin(); it!=t.end(); it++) {
        if (it!=t.begin())
            str += ",";
        str += asString(*it);
    }
    return str;
}


InClass::Type InClass::asType(const int i) {
    InClass::Type t;
    switch (i) {
        case -1: t = STOP;      break;
        case  0: t = NUMBER;    break;
        case  1: t = WORD;      break;
        case  2: t = TYPE;      break;
        case  3: t = QUALIF;    break;
        case  6: t = ROAD;      break;
        case  7: t = STOPWORD;  break;
        case  8: t = RR;        break;
        case  9: t = DASH;      break;
        case 10: t = CITY;      break;
        case 11: t = PROV;      break;
        case 12: t = NATION;    break;
        case 13: t = AMPERS;    break;
        case 14: t = BOXH;      break;
        case 15: t = ORD;       break;
        case 16: t = UNITH;     break;
        case 17: t = UNITT;     break;
        case 18: t = SINGLE;    break;
        case 19: t = BUILDH;    break;
        case 20: t = MILE;      break;
        case 21: t = DOUBLE;    break;
        case 22: t = DIRECT;    break;
        case 23: t = MIXED;     break;
        case 24: t = BUILDT;    break;
        case 25: t = FRACT;     break;
        case 26: t = PCT;       break;
        case 27: t = PCH;       break;
        case 28: t = QUINT;     break;
        case 29: t = QUAD;      break;
        case 30: t = PUNCT;     break;
        case 31: t = SPACE;     break;
        case 32: t = PLACEN;    break;
        case 33: t = EMDASH;    break;
        case 34: t = SLASH;     break;
        case 35: t = ATSIGN;    break;
        case 36: t = COMMA;     break;
        default: t = BADTOKEN;  break;
    }
    return t;
}


std::string InClass::asString(const InClass::Lang &lang) {
    std::string str;
    switch (lang) {
        case ALB:       str = "ALB"; break;
        case ARA:       str = "ARA"; break;
        case BAQ:       str = "BAQ"; break;
        case BEL:       str = "BEL"; break;
        case BET:       str = "BET"; break;
        case BEX:       str = "BEX"; break;
        case BOS:       str = "BOS"; break;
        case BOX:       str = "BOX"; break;
        case BUL:       str = "BUL"; break;
        case BUT:       str = "BUT"; break;
        case BUX:       str = "BUX"; break;
        case CAT:       str = "CAT"; break;
        case CHI:       str = "CHI"; break;
        case CHT:       str = "CHT"; break;
        case CZE:       str = "CZE"; break;
        case CZX:       str = "CZX"; break;
        case DAN:       str = "DAN"; break;
        case DUT:       str = "DUT"; break;
        case ENG:       str = "ENG"; break;
        case EST:       str = "EST"; break;
        case ESX:       str = "ESX"; break;
        case FIN:       str = "FIN"; break;
        case FRE:       str = "FRE"; break;
        case GER:       str = "GER"; break;
        case GLG:       str = "GLG"; break;
        case GRE:       str = "GRE"; break;
        case GRN:       str = "GRN"; break;
        case GRT:       str = "GRT"; break;
        case GRX:       str = "GRX"; break;
        case HEB:       str = "HEB"; break;
        case HUN:       str = "HUN"; break;
        case HUX:       str = "HUX"; break;
        case IND:       str = "IND"; break;
        case ITA:       str = "ITA"; break;
        case KOR:       str = "KOR"; break;
        case KOX:       str = "KOX"; break;
        case LAV:       str = "LAV"; break;
        case LAX:       str = "LAX"; break;
        case LIT:       str = "LIT"; break;
        case LIX:       str = "LIX"; break;
        case MAC:       str = "MAC"; break;
        case MAT:       str = "MAT"; break;
        case MAY:       str = "MAY"; break;
        case MLT:       str = "MLT"; break;
        case MLX:       str = "MLX"; break;
        case MNE:       str = "MNE"; break;
        case MNX:       str = "MNX"; break;
        case MOL:       str = "MOL"; break;
        case MOX:       str = "MOX"; break;
        case NOR:       str = "NOR"; break;
        case POL:       str = "POL"; break;
        case POR:       str = "POR"; break;
        case POX:       str = "POX"; break;
        case PYN:       str = "PYN"; break;
        case RMX:       str = "RMX"; break;
        case RST:       str = "RST"; break;
        case RUM:       str = "RUM"; break;
        case RUS:       str = "RUS"; break;
        case RUX:       str = "RUX"; break;
        case SCR:       str = "SCR"; break;
        case SCT:       str = "SCT"; break;
        case SCX:       str = "SCX"; break;
        case SIX:       str = "SIX"; break;
        case SLO:       str = "SLO"; break;
        case SLV:       str = "SLV"; break;
        case SLX:       str = "SLX"; break;
        case SPA:       str = "SPA"; break;
        case SRB:       str = "SRB"; break;
        case SRX:       str = "SRX"; break;
        case SWE:       str = "SWE"; break;
        case THA:       str = "THA"; break;
        case TKT:       str = "TKT"; break;
        case TUR:       str = "TUR"; break;
        case TUX:       str = "TUX"; break;
        case UKR:       str = "UKR"; break;
        case UKT:       str = "UKT"; break;
        case UKX:       str = "UKX"; break;
        case WEL:       str = "WEL"; break;
        case UNKNOWN:   str = "UNKNOWN"; break;
    }

    return str;
}


std::string InClass::asName(const InClass::Lang &lang) {
    std::string str;
    switch (lang) {
        case ALB:       str = "ALBANIAN"; break;
        case ARA:       str = "ARABIC"; break;
        case BAQ:       str = "BASQUE"; break;
        case BEL:       str = "BELARUSIAN"; break;
        case BET:       str = "BELARUSIAN_TRANSCRIBED"; break;
        case BEX:       str = "BELARUSIAN_TRANSLITERATED"; break;
        case BOS:       str = "BOSNIAN"; break;
        case BOX:       str = "BOSNIAN_TRANSLITERATED"; break;
        case BUL:       str = "BULGARIAN"; break;
        case BUT:       str = "BULGARIAN_TRANSCRIBED"; break;
        case BUX:       str = "BULGARIAN_TRANSLITERATED"; break;
        case CAT:       str = "CATALAN"; break;
        case CHI:       str = "CHINESE_MODERN"; break;
        case CHT:       str = "CHINESE_TRADITIONAL"; break;
        case CZE:       str = "CZECH"; break;
        case CZX:       str = "CZECH_TRANSLITERATED"; break;
        case DAN:       str = "DANISH"; break;
        case DUT:       str = "DUTCH"; break;
        case ENG:       str = "ENGLISH"; break;
        case EST:       str = "ESTONIAN"; break;
        case ESX:       str = "ESTONIAN_TRANSLITERATED"; break;
        case FIN:       str = "FINNISH"; break;
        case FRE:       str = "FRENCH"; break;
        case GER:       str = "GERMAN"; break;
        case GLG:       str = "GALICIAN"; break;
        case GRE:       str = "GREEK"; break;
        case GRN:       str = "GUARANI"; break;
        case GRT:       str = "GREEK_TRANSCRIBED"; break;
        case GRX:       str = "GREK_TRANSLITERATED"; break;
        case HEB:       str = "HEBREW"; break;
        case HUN:       str = "HUNGARIAN"; break;
        case HUX:       str = "HUNGARIAN_TRANSLITERATED"; break;
        case IND:       str = "BAHASA_INDONESIA"; break;
        case ITA:       str = "ITALIAN"; break;
        case KOR:       str = "KOREAN"; break;
        case KOX:       str = "KOREAN_TRANSLITERATED"; break;
        case LAV:       str = "LATVIAN"; break;
        case LAX:       str = "LATVIAN_TRANSLITERATED"; break;
        case LIT:       str = "LITHUANIAN"; break;
        case LIX:       str = "LITHUANIAN_TRANSLITERATED"; break;
        case MAC:       str = "MACEDONIAN"; break;
        case MAT:       str = "MACEDONIAN_TRANSCRIBED"; break;
        case MAY:       str = "MALAYSIAN"; break;
        case MLT:       str = "MALTESE"; break;
        case MLX:       str = "MALTESE_TRANSLITERATED"; break;
        case MNE:       str = "MONTENEGRIN"; break;
        case MNX:       str = "MONTENEGRIN_TRANSLITERATED"; break;
        case MOL:       str = "MOLDOVAN"; break;
        case MOX:       str = "MOLDOVAN_TRANSLITERATED"; break;
        case NOR:       str = "NORWEGIAN"; break;
        case POL:       str = "POLISH"; break;
        case POR:       str = "PORTUGUESE"; break;
        case POX:       str = "POLISH_TRANSLITERATED"; break;
        case PYN:       str = "PINYIN"; break;
        case RMX:       str = "ROMANIAN_TRANSLITERATED"; break;
        case RST:       str = "RUSSIAN_TRANSCRIBED"; break;
        case RUM:       str = "ROMANIAN"; break;
        case RUS:       str = "RUSSIAN"; break;
        case RUX:       str = "RUSSIAN_TRANSLITERATED"; break;
        case SCR:       str = "CROATIAN"; break;
        case SCT:       str = "SERBIAN_TRANSCRIBED"; break;
        case SCX:       str = "SERBIAN_TRANSLITERATED"; break;
        case SIX:       str = "SLOVENIAN_TRANSLITERATED"; break;
        case SLO:       str = "SLOVAK"; break;
        case SLV:       str = "SLOVENIAN"; break;
        case SLX:       str = "SLOVAK_TRANSLITERATED"; break;
        case SPA:       str = "SPANISH"; break;
        case SRB:       str = "SERBIAN"; break;
        case SRX:       str = "CROATION_TRANSLITERATED"; break;
        case SWE:       str = "SWEDISH"; break;
        case THA:       str = "THAI"; break;
        case TKT:       str = "TURKISH_TRANSCRIBED"; break;
        case TUR:       str = "TURKISH"; break;
        case TUX:       str = "TURKISH_TRANSLITERATED"; break;
        case UKR:       str = "UKRAINIAN"; break;
        case UKT:       str = "UKRAINIAN_TRANSCRIBED"; break;
        case UKX:       str = "UKRAINIAN_TRANSLITERATED"; break;
        case WEL:       str = "WELSH"; break;
        case UNKNOWN:   str = "UNKNOWN"; break;
    }

    return str;
}


InClass::Lang InClass::asLang(const std::string &s) {
    InClass::Lang lang;

    if      ( s == "ALB" ) lang = ALB;
    else if ( s == "ARA" ) lang = ARA;
    else if ( s == "BAQ" ) lang = BAQ;
    else if ( s == "BEL" ) lang = BEL;
    else if ( s == "BET" ) lang = BET;
    else if ( s == "BEX" ) lang = BEX;
    else if ( s == "BOS" ) lang = BOS;
    else if ( s == "BOX" ) lang = BOX;
    else if ( s == "BUL" ) lang = BUL;
    else if ( s == "BUT" ) lang = BUT;
    else if ( s == "BUX" ) lang = BUX;
    else if ( s == "CAT" ) lang = CAT;
    else if ( s == "CHI" ) lang = CHI;
    else if ( s == "CHT" ) lang = CHT;
    else if ( s == "CZE" ) lang = CZE;
    else if ( s == "CZX" ) lang = CZX;
    else if ( s == "DAN" ) lang = DAN;
    else if ( s == "DUT" ) lang = DUT;
    else if ( s == "ENG" ) lang = ENG;
    else if ( s == "EST" ) lang = EST;
    else if ( s == "ESX" ) lang = ESX;
    else if ( s == "FIN" ) lang = FIN;
    else if ( s == "FRE" ) lang = FRE;
    else if ( s == "GER" ) lang = GER;
    else if ( s == "GLG" ) lang = GLG;
    else if ( s == "GRE" ) lang = GRE;
    else if ( s == "GRN" ) lang = GRN;
    else if ( s == "GRT" ) lang = GRT;
    else if ( s == "GRX" ) lang = GRX;
    else if ( s == "HEB" ) lang = HEB;
    else if ( s == "HUN" ) lang = HUN;
    else if ( s == "HUX" ) lang = HUX;
    else if ( s == "IND" ) lang = IND;
    else if ( s == "ITA" ) lang = ITA;
    else if ( s == "KOR" ) lang = KOR;
    else if ( s == "KOX" ) lang = KOX;
    else if ( s == "LAV" ) lang = LAV;
    else if ( s == "LAX" ) lang = LAX;
    else if ( s == "LIT" ) lang = LIT;
    else if ( s == "LIX" ) lang = LIX;
    else if ( s == "MAC" ) lang = MAC;
    else if ( s == "MAT" ) lang = MAT;
    else if ( s == "MAY" ) lang = MAY;
    else if ( s == "MLT" ) lang = MLT;
    else if ( s == "MLX" ) lang = MLX;
    else if ( s == "MNE" ) lang = MNE;
    else if ( s == "MNX" ) lang = MNX;
    else if ( s == "MOL" ) lang = MOL;
    else if ( s == "MOX" ) lang = MOX;
    else if ( s == "NOR" ) lang = NOR;
    else if ( s == "POL" ) lang = POL;
    else if ( s == "POR" ) lang = POR;
    else if ( s == "POX" ) lang = POX;
    else if ( s == "PYN" ) lang = PYN;
    else if ( s == "RMX" ) lang = RMX;
    else if ( s == "RST" ) lang = RST;
    else if ( s == "RUM" ) lang = RUM;
    else if ( s == "RUS" ) lang = RUS;
    else if ( s == "RUX" ) lang = RUX;
    else if ( s == "SCR" ) lang = SCR;
    else if ( s == "SCT" ) lang = SCT;
    else if ( s == "SCX" ) lang = SCX;
    else if ( s == "SIX" ) lang = SIX;
    else if ( s == "SLO" ) lang = SLO;
    else if ( s == "SLV" ) lang = SLV;
    else if ( s == "SLX" ) lang = SLX;
    else if ( s == "SPA" ) lang = SPA;
    else if ( s == "SRB" ) lang = SRB;
    else if ( s == "SRX" ) lang = SRX;
    else if ( s == "SWE" ) lang = SWE;
    else if ( s == "THA" ) lang = THA;
    else if ( s == "TKT" ) lang = TKT;
    else if ( s == "TUR" ) lang = TUR;
    else if ( s == "TUX" ) lang = TUX;
    else if ( s == "UKR" ) lang = UKR;
    else if ( s == "UKT" ) lang = UKT;
    else if ( s == "UKX" ) lang = UKX;
    else if ( s == "WEL" ) lang = WEL;
    else if ( s == "ALBANIAN" )                     lang = ALB;
    else if ( s == "ARABIC" )                       lang = ARA;
    else if ( s == "BASQUE" )                       lang = BAQ;
    else if ( s == "BELARUSIAN" )                   lang = BEL;
    else if ( s == "BELARUSIAN_TRANSCRIBED" )       lang = BET;
    else if ( s == "BELARUSIAN_TRANSLITERATED" )    lang = BEX;
    else if ( s == "BOSNIAN" )                      lang = BOS;
    else if ( s == "BOSNIAN_TRANSLITERATED" )       lang = BOX;
    else if ( s == "BULGARIAN" )                    lang = BUL;
    else if ( s == "BULGARIAN_TRANSCRIBED" )        lang = BUT;
    else if ( s == "BULGARIAN_TRANSLITERATED" )     lang = BUX;
    else if ( s == "CATALAN" )                      lang = CAT;
    else if ( s == "CHINESE_MODERN" )               lang = CHI;
    else if ( s == "CHINESE_TRADITIONAL" )          lang = CHT;
    else if ( s == "CZECH" )                        lang = CZE;
    else if ( s == "CZECH_TRANSLITERATED" )         lang = CZX;
    else if ( s == "DANISH" )                       lang = DAN;
    else if ( s == "DUTCH" )                        lang = DUT;
    else if ( s == "ENGLISH" )                      lang = ENG;
    else if ( s == "ESTONIAN" )                     lang = EST;
    else if ( s == "ESTONIAN_TRANSLITERATED" )      lang = ESX;
    else if ( s == "FINNISH" )                      lang = FIN;
    else if ( s == "FRENCH" )                       lang = FRE;
    else if ( s == "GERMAN" )                       lang = GER;
    else if ( s == "GALICIAN" )                     lang = GLG;
    else if ( s == "GREEK" )                        lang = GRE;
    else if ( s == "GUARANI" )                      lang = GRN;
    else if ( s == "GREEK_TRANSCRIBED" )            lang = GRT;
    else if ( s == "GREK_TRANSLITERATED" )          lang = GRX;
    else if ( s == "HEBREW" )                       lang = HEB;
    else if ( s == "HUNGARIAN" )                    lang = HUN;
    else if ( s == "HUNGARIAN_TRANSLITERATED" )     lang = HUX;
    else if ( s == "BAHASA_INDONESIA" )             lang = IND;
    else if ( s == "ITALIAN" )                      lang = ITA;
    else if ( s == "KOREAN" )                       lang = KOR;
    else if ( s == "KOREAN_TRANSLITERATED" )        lang = KOX;
    else if ( s == "LATVIAN" )                      lang = LAV;
    else if ( s == "LATVIAN_TRANSLITERATED" )       lang = LAX;
    else if ( s == "LITHUANIAN" )                   lang = LIT;
    else if ( s == "LITHUANIAN_TRANSLITERATED" )    lang = LIX;
    else if ( s == "MACEDONIAN" )                   lang = MAC;
    else if ( s == "MACEDONIAN_TRANSCRIBED" )       lang = MAT;
    else if ( s == "MALAYSIAN" )                    lang = MAY;
    else if ( s == "MALTESE" )                      lang = MLT;
    else if ( s == "MALTESE_TRANSLITERATED" )       lang = MLX;
    else if ( s == "MONTENEGRIN" )                  lang = MNE;
    else if ( s == "MONTENEGRIN_TRANSLITERATED" )   lang = MNX;
    else if ( s == "MOLDOVAN" )                     lang = MOL;
    else if ( s == "MOLDOVAN_TRANSLITERATED" )      lang = MOX;
    else if ( s == "NORWEGIAN" )                    lang = NOR;
    else if ( s == "POLISH" )                       lang = POL;
    else if ( s == "PORTUGUESE" )                   lang = POR;
    else if ( s == "POLISH_TRANSLITERATED" )        lang = POX;
    else if ( s == "PINYIN" )                       lang = PYN;
    else if ( s == "ROMANIAN_TRANSLITERATED" )      lang = RMX;
    else if ( s == "RUSSIAN_TRANSCRIBED" )          lang = RST;
    else if ( s == "ROMANIAN" )                     lang = RUM;
    else if ( s == "RUSSIAN" )                      lang = RUS;
    else if ( s == "RUSSIAN_TRANSLITERATED" )       lang = RUX;
    else if ( s == "CROATIAN" )                     lang = SCR;
    else if ( s == "SERBIAN_TRANSCRIBED" )          lang = SCT;
    else if ( s == "SERBIAN_TRANSLITERATED" )       lang = SCX;
    else if ( s == "SLOVENIAN_TRANSLITERATED" )     lang = SIX;
    else if ( s == "SLOVAK" )                       lang = SLO;
    else if ( s == "SLOVENIAN" )                    lang = SLV;
    else if ( s == "SLOVAK_TRANSLITERATED" )        lang = SLX;
    else if ( s == "SPANISH" )                      lang = SPA;
    else if ( s == "SERBIAN" )                      lang = SRB;
    else if ( s == "CROATION_TRANSLITERATED" )      lang = SRX;
    else if ( s == "SWEDISH" )                      lang = SWE;
    else if ( s == "THAI" )                         lang = THA;
    else if ( s == "TURKISH_TRANSCRIBED" )          lang = TKT;
    else if ( s == "TURKISH" )                      lang = TUR;
    else if ( s == "TURKISH_TRANSLITERATED" )       lang = TUX;
    else if ( s == "UKRAINIAN" )                    lang = UKR;
    else if ( s == "UKRAINIAN_TRANSCRIBED" )        lang = UKT;
    else if ( s == "UKRAINIAN_TRANSLITERATED" )     lang = UKX;
    else if ( s == "WELSH" )                        lang = WEL;
    else                                            lang = UNKNOWN;

    return lang;
}


std::set<InClass::AttachType> InClass::asAttachType(const std::string &s) {
    std::set<InClass::AttachType> type;
    std::stringstream buffer(s);
    std::string word;
    while (true) {
        std::getline(buffer, word, ',');
        word = trimboth( word );
        if (word.length() > 0) {
            if      ( word == "DET_PRE" ) type.insert( DET_PRE );
            else if ( word == "DET_SUF" ) type.insert( DET_SUF );
            else if ( word == "ATT_PRE" ) type.insert( ATT_PRE );
            else if ( word == "ATT_SUF" ) type.insert( ATT_SUF );
            // other tokens are ignored and not added to the set
        }
        if (buffer.eof())
            break;
    }
    return type;
}

std::string InClass::asString(const InClass::AttachType &t) {
    std::string str;
    switch (t) {
        case DET_PRE: str = "DET_PRE"; break;
        case DET_SUF: str = "DET_SUF"; break;
        case ATT_PRE: str = "ATT_PRE"; break;
        case ATT_SUF: str = "ATT_SUF"; break;
    }

    return str;
}


std::string InClass::asString(const std::set<InClass::AttachType> &t) {
    std::string str;
    for (auto it=t.begin(); it!=t.end(); it++) {
        if (it!=t.begin())
            str += ",";
        str += asString(*it);
    }
    return str;
}
