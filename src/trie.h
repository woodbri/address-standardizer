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


#ifndef TRIE_H
#define TRIE_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

typedef std::set<std::string> WordSet;

class Trie;

typedef std::map<char, Trie*> ChildMap;

class Trie {

public:
    Trie( bool end = false ) :_size( 0 ), _isEnd( end ) {};
    ~Trie() {
        for (auto &it : _children)
            delete it.second;
    };

    void addWord( std::string word );

    bool isPrefix( const std::string &pref ) const;
    bool isWord( const std::string &word ) const;
    size_t size() const { return _size; };
    void getWords( WordSet &words, std::string wordSoFar="" ) const;
    void getWordsStartingWith( std::string prefix, WordSet &words, std::string wordSoFar="" ) const;
    std::string quoteChar( const char c ) const;
    std::string getRegexp() const;
    friend std::ostream &operator<<( std::ostream &ss, const Trie &t );


private:
    ChildMap _children;
    size_t _size;
    bool _isEnd;

};

#endif
