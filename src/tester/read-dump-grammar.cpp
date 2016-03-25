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
// test the grammar reader and check()

#include <iostream>
#include <string>

#include "grammar.h"
#include "utils.h"


int main(int ac, char* av[])
{
    if (ac < 2) {
        std::cerr << "Usage: read-dump-grammar test.grammar\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    Grammar g(file);

    std::cout << g << "\n";

    return EXIT_SUCCESS;
}
