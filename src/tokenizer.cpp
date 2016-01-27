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

#include <vector>
#include <string>

#include "tokenizer.h"


std::vector<Token> Tokenizer::getTokens(std::string str) {


    // first see if we have attached tokens that we need to split apart
    // we place and em dash utf8 char "\xe2\x80\x94" where we split the word
    // this can later be recognized as EMDASH token if needed
    std::string attached = "(" + lex_.regexPrefixAtt() + ")";
    if (attached.length() > 2) {
        boost::u32regex re = boost::make_u32regex( attached );
        const char* replace( "$1\xe2\x80\x94" );
        std::string tmp = boost::u32regex_replace( str, re, replace );
        str = tmp;
    }

    attached = "(" + lex_.regexSuffixAtt() + ")";
    if (attached.length() > 2) {
        boost::u32regex re = boost::make_u32regex( attached );
        const char* replace( "\xe2\x80\x94$1" );
        std::string tmp = boost::u32regex_replace( str, re, replace );
        str = tmp;
    }

    // build the regex for identifying tokens
    std::string regex = "^\\s*(?!(?:\xe2\x80\x94)+)(" + lex_.regex() + "\\d+/\\d+|\\d+|\\<[[:alpha:]]+\\>|[\\p{L}\\p{Nd}]+)([-&\\s\\|[:punct:]]+|\xe2\x80\x94|$)";

    boost::u32regex re = boost::make_u32regex( regex, boost::regex::icase );

    std::vector<Token> outtokens;

    boost::smatch what;
    boost::match_flag_type flags = boost::match_default;

    // could not get this to work with auto
    std::string::const_iterator start, end;
    start = str.begin();
    end   = str.end();

    while ( boost::u32regex_search( start, end, what, re ) ) {
        // fetch and create the token for the word or phrase
        if (what[1].first < what[1].second) {
            Token tok( std::string(what[1].first, what[1].second) );
            // token might classify as multiple types
            // or none, in which case make it a word
            lex_.classify(tok, InClass::WORD);
            if (not tok.isInClass(filter_))
                outtokens.push_back(tok);
        }

        // create a token for the punctuation
        if (what[2].first < what[2].second) {
            Token punct( std::string(what[2].first, what[2].second) );
            lex_.classify(punct, InClass::PUNCT);
            if (not punct.isInClass(filter_))
                outtokens.push_back(punct);
        }

        // update search position
        start = what[0].second;

        // update flags
        flags |= boost::match_not_bob;
    }

    return outtokens;
}

