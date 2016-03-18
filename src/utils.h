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

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <map>
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

    static void count( const std::string );
    static void clear( const std::string );
    static unsigned long int getCount( const std::string );

private:

    static std::map<std::string, unsigned long int> counts_;

};

#endif

