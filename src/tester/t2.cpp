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
#include "grammar.h"
#include "search.h"

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <chrono>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/archive_exception.hpp>

int main(int ac, char* av[]) {

    if (ac < 4 or ac > 5) {
        std::cerr << "Usage: t2 lex.txt grammar.txt 'address to parse' ['filter']\n";
        return EXIT_FAILURE;
    }

    std::string lfile = av[1];
    std::string gfile = av[2];
    std::string str = av[3];
    std::string filterstr("PUNCT,SPACE,EMDASH,STOPWORD");
    if (ac == 5)
        filterstr = av[4];


    //std::cout << "Lexicon: '" << file << "'\n";
    std::cout << "Address: '" << str << "'\n";

    // Normalize and UPPERCASE the input string
    UErrorCode errorCode;
    std::string nstr = Utils::normalizeUTF8( str, errorCode );
    std::string Ustr = Utils::upperCaseUTF8( nstr, "en" );

    std::cout << "Normalized: '" << nstr << "'\n";
    std::cout << "UpperCase: '" << Ustr << "'\n";

    //Lexicon lex("test-lex", lfile);
    auto t0 = std::chrono::system_clock::now();

    // simulate the postgres call, by reading the whole file into a string
    std::istringstream iss;
    std::string s;
    std::ifstream t(lfile);
    if ( t.fail() ) {
        std::cout << "ERROR: failed to open '" << lfile << "' for read!\n";
        return 1;
    }
    t.seekg(0, std::ios::end);
    s.clear();
    std::cout << "t.tellg() = " << t.tellg() << "\n";
    s.reserve(t.tellg());
    t.seekg(0, std::ios::beg);
    s.assign((std::istreambuf_iterator<char>(t)),
              std::istreambuf_iterator<char>());
    iss.str( s );
    //std::cout << "Lexicon size: " << s.size() << "\n";

    auto t1 = std::chrono::system_clock::now();
    auto diff = t1 - t0;
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Timer: read lexicon: " << dt.count() << " ms\n";

    t0 = std::chrono::system_clock::now();
    // Lexicon lex( "query-lex", iss );
    Lexicon lex;
    try {
        boost::archive::text_iarchive ia(iss);
        ia >> lex;
    }
    catch (std::runtime_error& e) {
        if ( e.what() == std::string("Re-compile-lexicon"))
            throw;
        std::cout << "Load from serialized failed! Trying to load lexicon.\n";
        iss.clear();
        iss.str( s );
        lex.name( "query-lex" );
        lex.initialize( iss );
    }
    catch ( ... ) {
        std::cout << "Load from serialized failed! Trying to load lexicon.\n";
        iss.clear();
        iss.str( s );
        lex.name( "query-lex" );
        lex.initialize( iss );
    }
    t1 = std::chrono::system_clock::now();
    diff = t1 - t0;
    dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Timer: load lexicon: " << dt.count() << " ms\n";

    std::cout << "Lexicon md5: " << lex.getMd5() << "\n";

//    std::cout << lex << "\n";
//    std::cout << "Lexicon regex: '" << lex.regex() <<"'\n\n";
//    std::cout << "Lexicon attachedRegex: '" << lex.attachedRegex() <<"'\n\n";

    t0 = std::chrono::system_clock::now();
    Tokenizer tokenizer( lex );
    tokenizer.filter( InClass::asType( filterstr ) );
    t1 = std::chrono::system_clock::now();
    diff = t1 - t0;
    dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Timer: init tokenizer: " << dt.count() << " ms\n";

    std::vector<std::vector<Token> > phrases;
    t0 = std::chrono::system_clock::now();
    phrases.push_back( tokenizer.getTokens( Ustr ) );
    t1 = std::chrono::system_clock::now();
    diff = t1 - t0;
    dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
    std::cout << "Timer: tokenizer::getTokens: " << dt.count() << " ms\n";

    for ( const auto phrase : phrases ) {
        std::cout << "-------------------------------\n";
        for ( const auto tok : phrase )
            std::cout << tok << "\n";
    }
    std::cout << "-------------------------------\n";

    try {
        //Grammar G( gfile );

        // simulate the postgres call, by reading the whole file into a string
        t0 = std::chrono::system_clock::now();
        std::ifstream t(gfile);
        if ( t.fail() ) {
            std::cout << "ERROR: failed to open '" << gfile << "' for read!\n";
            return 1;
        }
        t.seekg(0, std::ios::end);
        s.reserve(t.tellg());
        t.seekg(0, std::ios::beg);
        s.assign((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());

        //std::cout << "Grammar size: " << s.size() << "\n";
        //std::cout << s <<"\n";

        iss.clear();
        iss.str( s );
        t1 = std::chrono::system_clock::now();
        diff = t1 - t0;
        dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        std::cout << "Timer: read grammar file: " << dt.count() << " ms\n";

        t0 = std::chrono::system_clock::now();
        Grammar G( iss );
        t1 = std::chrono::system_clock::now();
        diff = t1 - t0;
        dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        std::cout << "Timer: init Grammar: " << dt.count() << " ms\n";

        switch (G.status() ) {
            case Grammar::CHECK_FATAL:
                std::cout << "FATAL loading grammar: " << G.issues() << "\n";
                return(1);
            case Grammar::CHECK_WARN:
                std::cout << "WARNING loading grammar: " << G.issues() << "\n";
                break;
            case Grammar::CHECK_OK:
                std::cout << "OK loading grammar: " << G.issues() << "\n";
                break;
        }

        std::cout << "Grammar md5: " << G.getMd5() << "\n";

        //std::cout << "--Gramar-------------------\n" << G << "---------------------------\n";

        Search S( G );

        float bestCost = -1.0;
        t0 = std::chrono::system_clock::now();
        auto best = S.searchAndReclassBest( phrases, bestCost );
        t1 = std::chrono::system_clock::now();
        diff = t1 - t0;
        dt = std::chrono::duration_cast<std::chrono::milliseconds>(diff);
        std::cout << "Timer: Search::searchAndReclassBest: " << dt.count() << " ms\n";

        std::cout << "Stats: findMetas: " << Utils::getCount("findMetas") << "\n";
        std::cout << "Stats: findRules: " << Utils::getCount("findRules") << "\n";


        if ( bestCost < 0.0 ) {
            std::cout << "#### Failed to find a match (searchAndReclassBest)(" << bestCost << ")\n";
            return(1);
        }

        // get the appropriate standard terms and
        // print out the reclassified tokens
        std::cout << "bestCost: " << bestCost << "\n";
        for (auto &token : best) {
            lex.standardize( token );
            std::cout << token << "\n";
        }

    }
    catch (std::runtime_error& e) {
        std::cout << "Runtime Error: " << e.what() << "\n";
        return(1);
    }
    catch (...) {
        std::cout << "Error: Unknown exception caught!\n";
        return(1);
    }
    return EXIT_SUCCESS;
}
