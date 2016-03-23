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
#include <iostream>
#include <string>
#include <boost/regex.hpp>      // Boost.Regex lib
#include <boost/regex/icu.hpp>  // Boost.Regex for unicode
#include "utils.h"

using namespace std;

std::map<std::string, unsigned long int> Utils::counts_;

int main( ) {

   std::string str, sre;
   boost::regex re;

   while(true)
   {
      cout << "Expression: ";
      cin >> sre;
      if (sre == "quit")
      {
         break;
      }
      cout << "String:     ";
      cin >> str;

      // Normalize and UPPERCASE the input string
      UErrorCode errorCode;
      std::string nstr = Utils::normalizeUTF8( str, errorCode );

      std::string nre = Utils::normalizeUTF8( sre, errorCode );
      boost::u32regex re = boost::make_u32regex( nre );

      auto start = nstr.begin();
      auto end   = nstr.end();

      if (boost::u32regex_match(start, end, re))
      {
         cout << "'" << nre << "' matches '" << nstr << "'" << endl;
      }
      else {
         cout << "'" << nre << "' does not match '" << nstr << "'" << endl;
      }
   }
}
