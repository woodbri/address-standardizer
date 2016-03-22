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


#include "inclass.h"
#include "lexicon.h"
#include "token.h"
#include "tokenizer.h"
#include "utils.h"

#include <iostream>
#include <string>
#include <vector>

std::map<std::string, unsigned long int> Utils::counts_;

int main(int ac, char* av[]) {

    if (ac < 3) {
        std::cerr << "Usage: t2 lex.txt 'address to parse'\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];
    std::string str = av[2];

    std::cout << "Lexicon: '" << file << "'\n";
    std::cout << "Address: '" << str << "'\n";

    // Normalize and UPPERCASE the input string
    UErrorCode errorCode;
    std::string nstr = Utils::normalizeUTF8( str, errorCode );
    std::string Ustr = Utils::upperCaseUTF8( nstr, "en" );

    std::cout << "Normalized: '" << nstr << "'\n";
    std::cout << "UpperCase: '" << Ustr << "'\n";

    std::vector<Token> tokens;

    Lexicon lex("test-lex", file);
    std::cout << lex << "\n";
    std::cout << "Lexicon regex: '" << lex.regex() <<"'\n\n";
    std::cout << "Lexicon regexPrefixAtt: '" << lex.regexPrefixAtt() <<"'\n\n";
    std::cout << "Lexicon regexSuffixAtt: '" << lex.regexSuffixAtt() <<"'\n\n";

    Tokenizer tokenizer( lex );
    tokenizer.addFilter( InClass::PUNCT );
    tokens = tokenizer.getTokens( Ustr );

    std::cout << "tokens.size() = " << tokens.size() << "\n";

    for (const auto tok : tokens)
        std::cout << tok << "\n";

    return EXIT_SUCCESS;
}
