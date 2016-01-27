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


#include "lexicon.h"

#include <iostream>
#include <string>
#include <vector>

int main(int ac, char* av[]) {

    if (ac != 2) {
        std::cerr << "Usage: t1 lex.txt\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    std::cout << "file: '" << file << "'\n";

    Lexicon lex("test-lex", file);

    std::cout << lex << "\n\n";

    return EXIT_SUCCESS;
}
