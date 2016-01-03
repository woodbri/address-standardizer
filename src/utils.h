/*HEADER*
*HEADER*/

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unicode/utypes.h>
#include <unicode/uchar.h>
#include <unicode/locid.h>
#include <unicode/ustring.h>
#include <unicode/unistr.h>
#include <unicode/translit.h>
#include <unicode/normalizer2.h>


class Utils {

public:

    static std::string unaccentUTF8( const std::string &str );
    static std::string upperCaseUTF8( const std::string &str, const std::string lang );
    static std::string normalizeUTF8( const std::string &str, UErrorCode &errorCode);

};

#endif

