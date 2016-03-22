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
// test the lexicon reader and dump

#include <iostream>
#include <string>

#include "lexicon.h"
#include "utils.h"

std::map<std::string, unsigned long int> Utils::counts_;

int main(int ac, char* av[])
{
    if (ac < 2) {
        std::cerr << "Usage: read-dump-lexicon lexicon.txt\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    Lexicon lex(file, file);

    std::cout << lex << "\n";

    return EXIT_SUCCESS;
}
