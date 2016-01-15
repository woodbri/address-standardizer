/*HEADER*
*HEADER*/

#include <sstream>

#include "inclass.h"

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
        default:        str = "BADTOKEN"; break;
    };
    return str;
}

std::set<InClass::Type> InClass::asType(const std::string &s) {
    std::set<InClass::Type> ret;
    InClass::Type t;
    std::map<std::string, InClass::Type> m;
    std::map<std::string, InClass::Type>::iterator it;

    m["STOP"]      = STOP;
    m["NUMBER"]    = NUMBER;
    m["WORD"]      = WORD;
    m["TYPE"]      = TYPE;
    m["QUALIF"]    = QUALIF;
    m["ROAD"]      = ROAD;
    m["STOPWORD"]  = STOPWORD;
    m["RR"]        = RR;
    m["DASH"]      = DASH;
    m["CITY"]      = CITY;
    m["PROV"]      = PROV;
    m["NATION"]    = NATION;
    m["AMPERS"]    = AMPERS;
    m["BOXH"]      = BOXH;
    m["ORD"]       = ORD;
    m["UNITH"]     = UNITH;
    m["SINGLE"]    = SINGLE;
    m["BUILDH"]    = BUILDH;
    m["MILE"]      = MILE;
    m["DOUBLE"]    = DOUBLE;
    m["DIRECT"]    = DIRECT;
    m["MIXED"]     = MIXED;
    m["BUILDT"]    = BUILDT;
    m["FRACT"]     = FRACT;
    m["PCT"]       = PCT;
    m["PCH"]       = PCH;
    m["QUINT"]     = QUINT;
    m["QUAD"]      = QUAD;
    m["PUNCT"]     = PUNCT;
    m["SPACE"]     = SPACE;
    m["PLACEN"]    = PLACEN;
    m["BADTOKEN"]  = BADTOKEN;

    std::stringstream buffer(s);
    std::string word;
    while (true) {
        std::getline(buffer, word, ',');
        if (word.length() > 0) {
            it = m.find( word );
            if (it == m.end())
                t = BADTOKEN;
            else
                t = it->second;

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
    std::map<std::string, InClass::Lang> m;
    std::map<std::string, InClass::Lang>::iterator it;

    m["ALB"] = ALB;
    m["ARA"] = ARA;
    m["BAQ"] = BAQ;
    m["BEL"] = BEL;
    m["BET"] = BET;
    m["BEX"] = BEX;
    m["BOS"] = BOS;
    m["BOX"] = BOX;
    m["BUL"] = BUL;
    m["BUT"] = BUT;
    m["BUX"] = BUX;
    m["CAT"] = CAT;
    m["CHI"] = CHI;
    m["CHT"] = CHT;
    m["CZE"] = CZE;
    m["CZX"] = CZX;
    m["DAN"] = DAN;
    m["DUT"] = DUT;
    m["ENG"] = ENG;
    m["EST"] = EST;
    m["ESX"] = ESX;
    m["FIN"] = FIN;
    m["FRE"] = FRE;
    m["GER"] = GER;
    m["GLG"] = GLG;
    m["GRE"] = GRE;
    m["GRN"] = GRN;
    m["GRT"] = GRT;
    m["GRX"] = GRX;
    m["HEB"] = HEB;
    m["HUN"] = HUN;
    m["HUX"] = HUX;
    m["IND"] = IND;
    m["ITA"] = ITA;
    m["KOR"] = KOR;
    m["KOX"] = KOX;
    m["LAV"] = LAV;
    m["LAX"] = LAX;
    m["LIT"] = LIT;
    m["LIX"] = LIX;
    m["MAC"] = MAC;
    m["MAT"] = MAT;
    m["MAY"] = MAY;
    m["MLT"] = MLT;
    m["MLX"] = MLX;
    m["MNE"] = MNE;
    m["MNX"] = MNX;
    m["MOL"] = MOL;
    m["MOX"] = MOX;
    m["NOR"] = NOR;
    m["POL"] = POL;
    m["POR"] = POR;
    m["POX"] = POX;
    m["PYN"] = PYN;
    m["RMX"] = RMX;
    m["RST"] = RST;
    m["RUM"] = RUM;
    m["RUS"] = RUS;
    m["RUX"] = RUX;
    m["SCR"] = SCR;
    m["SCT"] = SCT;
    m["SCX"] = SCX;
    m["SIX"] = SIX;
    m["SLO"] = SLO;
    m["SLV"] = SLV;
    m["SLX"] = SLX;
    m["SPA"] = SPA;
    m["SRB"] = SRB;
    m["SRX"] = SRX;
    m["SWE"] = SWE;
    m["THA"] = THA;
    m["TKT"] = TKT;
    m["TUR"] = TUR;
    m["TUX"] = TUX;
    m["UKR"] = UKR;
    m["UKT"] = UKT;
    m["UKX"] = UKX;
    m["WEL"] = WEL;
    m["ALBANIAN"] = ALB;
    m["ARABIC"] = ARA;
    m["BASQUE"] = BAQ;
    m["BELARUSIAN"] = BEL;
    m["BELARUSIAN_TRANSCRIBED"] = BET;
    m["BELARUSIAN_TRANSLITERATED"] = BEX;
    m["BOSNIAN"] = BOS;
    m["BOSNIAN_TRANSLITERATED"] = BOX;
    m["BULGARIAN"] = BUL;
    m["BULGARIAN_TRANSCRIBED"] = BUT;
    m["BULGARIAN_TRANSLITERATED"] = BUX;
    m["CATALAN"] = CAT;
    m["CHINESE_MODERN"] = CHI;
    m["CHINESE_TRADITIONAL"] = CHT;
    m["CZECH"] = CZE;
    m["CZECH_TRANSLITERATED"] = CZX;
    m["DANISH"] = DAN;
    m["DUTCH"] = DUT;
    m["ENGLISH"] = ENG;
    m["ESTONIAN"] = EST;
    m["ESTONIAN_TRANSLITERATED"] = ESX;
    m["FINNISH"] = FIN;
    m["FRENCH"] = FRE;
    m["GERMAN"] = GER;
    m["GALICIAN"] = GLG;
    m["GREEK"] = GRE;
    m["GUARANI"] = GRN;
    m["GREEK_TRANSCRIBED"] = GRT;
    m["GREK_TRANSLITERATED"] = GRX;
    m["HEBREW"] = HEB;
    m["HUNGARIAN"] = HUN;
    m["HUNGARIAN_TRANSLITERATED"] = HUX;
    m["BAHASA_INDONESIA"] = IND;
    m["ITALIAN"] = ITA;
    m["KOREAN"] = KOR;
    m["KOREAN_TRANSLITERATED"] = KOX;
    m["LATVIAN"] = LAV;
    m["LATVIAN_TRANSLITERATED"] = LAX;
    m["LITHUANIAN"] = LIT;
    m["LITHUANIAN_TRANSLITERATED"] = LIX;
    m["MACEDONIAN"] = MAC;
    m["MACEDONIAN_TRANSCRIBED"] = MAT;
    m["MALAYSIAN"] = MAY;
    m["MALTESE"] = MLT;
    m["MALTESE_TRANSLITERATED"] = MLX;
    m["MONTENEGRIN"] = MNE;
    m["MONTENEGRIN_TRANSLITERATED"] = MNX;
    m["MOLDOVAN"] = MOL;
    m["MOLDOVAN_TRANSLITERATED"] = MOX;
    m["NORWEGIAN"] = NOR;
    m["POLISH"] = POL;
    m["PORTUGUESE"] = POR;
    m["POLISH_TRANSLITERATED"] = POX;
    m["PINYIN"] = PYN;
    m["ROMANIAN_TRANSLITERATED"] = RMX;
    m["RUSSIAN_TRANSCRIBED"] = RST;
    m["ROMANIAN"] = RUM;
    m["RUSSIAN"] = RUS;
    m["RUSSIAN_TRANSLITERATED"] = RUX;
    m["CROATIAN"] = SCR;
    m["SERBIAN_TRANSCRIBED"] = SCT;
    m["SERBIAN_TRANSLITERATED"] = SCX;
    m["SLOVENIAN_TRANSLITERATED"] = SIX;
    m["SLOVAK"] = SLO;
    m["SLOVENIAN"] = SLV;
    m["SLOVAK_TRANSLITERATED"] = SLX;
    m["SPANISH"] = SPA;
    m["SERBIAN"] = SRB;
    m["CROATION_TRANSLITERATED"] = SRX;
    m["SWEDISH"] = SWE;
    m["THAI"] = THA;
    m["TURKISH_TRANSCRIBED"] = TKT;
    m["TURKISH"] = TUR;
    m["TURKISH_TRANSLITERATED"] = TUX;
    m["UKRAINIAN"] = UKR;
    m["UKRAINIAN_TRANSCRIBED"] = UKT;
    m["UKRAINIAN_TRANSLITERATED"] = UKX;
    m["WELSH"] = WEL;

    it = m.find(s);
    if (it == m.end())
        lang = UNKNOWN;
    else
        lang = it->second;

    return lang;
}


std::set<InClass::AttachType> InClass::asAttachType(const std::string &s) {
    std::set<InClass::AttachType> type;
    std::stringstream buffer(s);
    std::string word;
    while (true) {
        std::getline(buffer, word, ',');
        if (word.length() > 0) {
            if      ( word == "DETACH" ) type.insert( DETACH );
            else if ( word == "PREFIX" ) type.insert( PREFIX );
            else if ( word == "SUFFIX" ) type.insert( SUFFIX );
            else type.insert( DETACH );
        }
        if (buffer.eof())
            break;
    }
    return type;
}

std::string InClass::asString(const InClass::AttachType &t) {
    std::string str;
    switch (t) {
        case DETACH: str = "DETACH"; break;
        case PREFIX: str = "PREFIX"; break;
        case SUFFIX: str = "SUFFIX"; break;
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
