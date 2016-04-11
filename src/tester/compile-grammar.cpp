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
// compile-grammar.cpp
// read a grammar file, build a trie, serialize trie to file.

#include <fstream>
#include <iostream>
#include <string>

#include "grammar.h"
#include "utils.h"


int main(int ac, char* av[])
{
    if (ac < 3) {
        std::cerr << "Usage: compile-grammar test.gmr test.tri\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    Grammar g(file);

    std::ofstream ofs( av[2], std::ofstream::out | std::ofstream::trunc
        | std::ofstream::binary );
    if ( ofs.good() ) {
        g.serialize( ofs );
        ofs.close();
    }
    else {
        std::cerr << "ERROR: failed to create '" << av[2] << "' for write!\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
