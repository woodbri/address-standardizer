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
#include <fstream>
#include <string>
#include <time.h>

#include <boost/archive/text_wiarchive.hpp>
#include <boost/archive/text_woarchive.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

//#include <boost/archive/binary_woarchive.hpp>
//#include <boost/archive/binary_wiarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/archive_exception.hpp>
#include <boost/archive/codecvt_null.hpp>

#include "lexicon.h"
#include "utils.h"

void save_lexicon(const Lexicon &lex, const char * filename)
{
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << lex;
}

void  restore_lexicon(Lexicon &lex, const char * filename)
{
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);
    ia >> lex;
}

void save_lexicon_w(const Lexicon &lex, const char * filename)
{
    std::wofstream ofs(filename);
    boost::archive::text_woarchive oa(ofs, 2);
    oa << lex;
}

void  restore_lexicon_w(Lexicon &lex, const char * filename)
{
    std::wifstream ifs(filename);
    boost::archive::text_wiarchive ia(ifs, 2);
    ia >> lex;
}

void save_lexicon_b(const Lexicon &lex, const char * filename)
{
    //std::wofstream os(filename, std::wios::out|std::wios::trunc|std::wios::binary);
    //std::wofstream os(filename, std::wios::out|std::wios::trunc|std::wios::binary);
    ////boost::archive::codecvt_null<wchar_t> loc;
    ////os.imbue(loc);
    //boost::archive::binary_oarchive oa(os, boost::archive::no_codecvt);
    //boost::archive::binary_woarchive oa(os, 2);
    std::ofstream os(filename, std::ios::out|std::ios::trunc|std::ios::binary);
    boost::archive::binary_oarchive oa(os, 2);
    oa << lex;
}

void restore_lexicon_b(Lexicon &lex, const char * filename)
{
    //std::wifstream is(filename, std::wios::in|std::wios::binary);
    ////boost::archive::codecvt_null<wchar_t> loc;
    ////is.imbue(loc);
    //boost::archive::binary_iarchive ia(is, boost::archive::no_codecvt);
    //boost::archive::binary_wiarchive ia(is, 2);
    std::ifstream is(filename, std::ios::in|std::ios::binary);
    boost::archive::binary_iarchive ia(is, 2);
    ia >> lex;
}

int main(int ac, char* av[])
{
    clock_t t, t0, t1;
    if (ac < 2) {
        std::cerr << "Usage: read-dump-lexicon lexicon.txt\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    t0 = clock();
    int i = 0;
    while (1) {
        Lexicon lex(file, file);
        ++i;
        if (i >= 10) break;
    }
    t1 = clock();
    t = t1 - t0;
    printf( "load 10 times: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    printf( "\n------- text save/load --------\n" );

    std::string serialfile = "lex-serial.txt";

    t0 = clock();
    Lexicon lex(file, file);
    t1 = clock();
    t = t1 - t0;
    printf( "load time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    t0 = clock();
    save_lexicon(lex, serialfile.c_str());
    t1 = clock();
    t = t1 - t0;
    printf( "save time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    Lexicon lex2;
    t0 = clock();
    restore_lexicon( lex2, serialfile.c_str() );
    t1 = clock();
    t = t1 - t0;
    printf( "retore time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    printf( "\n------- textw save/load --------\n" );

    serialfile = "lex-serial-w.txt";

    t0 = clock();
    Lexicon lex5(file, file);
    t1 = clock();
    t = t1 - t0;
    printf( "load time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    t0 = clock();
    save_lexicon(lex5, serialfile.c_str());
    t1 = clock();
    t = t1 - t0;
    printf( "save time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    Lexicon lex6;
    t0 = clock();
    restore_lexicon( lex6, serialfile.c_str() );
    t1 = clock();
    t = t1 - t0;
    printf( "retore time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    printf( "\n------- binary save/load --------\n" );

    serialfile = "lex-serial.dat";

    t0 = clock();
    Lexicon lex3(file, file);
    t1 = clock();
    t = t1 - t0;
    printf( "load time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    t0 = clock();
    save_lexicon_b(lex3, serialfile.c_str());
    t1 = clock();
    t = t1 - t0;
    printf( "save time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);

    Lexicon lex4;
    t0 = clock();
    restore_lexicon_b( lex4, serialfile.c_str() );
    t1 = clock();
    t = t1 - t0;
    printf( "retore time: %ld clicks, %f secs\n", t, ((float) t)/CLOCKS_PER_SEC);


    //std::cout << lex << "\n";

    return EXIT_SUCCESS;
}
