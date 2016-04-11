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
// test the grammar trie reader

#include <fstream>
#include <iostream>
#include <string>

#include "grammartrie.h"


int main(int ac, char* av[])
{
    if (ac < 2) {
        std::cerr << "Usage: read-grammar-trie test.tri\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    GrammarTrie gt;
    std::ifstream ifs( file, std::ifstream::in | std::ofstream::binary );
    if ( ifs.good() ) {
        gt.deSerialize( ifs );
        ifs.close();
    }
    else {
        std::cerr << "ERROR: failed to open '" << file << "' for read!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
