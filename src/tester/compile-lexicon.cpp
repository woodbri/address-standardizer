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
// compile-lexicon.cpp
// read a lexicon file, build regex expressions, serialize to file.

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/archive_exception.hpp>

#include "lexicon.h"


int main(int ac, char* av[])
{
    if (ac < 3) {
        std::cerr << "Usage: compile-lexicon test.lex test.clex\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];
    Lexicon lex( "lexicon", file );

    // populate the regex strings in the Lexicon
    lex.regex();
    lex.regexPrefixAtt();
    lex.regexSuffixAtt();

    std::ofstream ofs( av[2], std::ofstream::out | std::ofstream::trunc
        | std::ofstream::binary );
    if ( ofs.good() ) {
        boost::archive::text_oarchive oa(ofs);
        oa << lex;
        ofs.close();
    }
    else {
        std::cerr << "ERROR: failed to create '" << av[2] << "' for write!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
