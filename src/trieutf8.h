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


#ifndef TRIEUTF8_H
#define TRIEUTF8_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "utf8iterator.h"


typedef std::set<std::string> WordSet;

class TrieUtf8;

typedef std::map<char32_t, TrieUtf8*> ChildMap;

class TrieUtf8 {

public:
    TrieUtf8( bool end = false ) :_size( 0 ), _isEnd( end ) {};
    ~TrieUtf8() {
        for (auto &it : _children)
            delete it.second;
    };

    void addWord( const std::string &word );

    bool isPrefix( const std::string &pref ) const;
    bool isWord( const std::string &word ) const;
    size_t size() const { return _size; };
    void getWords( WordSet &words, std::string wordSoFar="" ) const;
    void getWordsStartingWith( const std::string &prefix, WordSet &words, std::string wordSoFar="" ) const;
    std::string quoteMeta( const std::string &str ) const;
    std::string getRegexp() const;
    friend std::ostream &operator<<( std::ostream &ss, const TrieUtf8 &t );


private:
    ChildMap _children;
    size_t _size;
    bool _isEnd;

};

#endif
