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


// This code was adapted from this gist:
// https://gist.github.com/woodbri/2f0044d93c5ad2307783
// And I added getRegexp() based on Perl Regexp::Trie module
// I've modified it to work with UTF-8 strings

#include "trieutf8.h"


void TrieUtf8::addWord( const std::string &word ) {
    Utf8Iterator it = word.begin();
    Utf8Iterator end = word.end();
    if ( it != end ) {
        ++_size;
        // get the code point for the trie
        char32_t c = *it;
        ++it;  // step to the next code point
        std::string subword = it.substr( end );
        if ( _children[ c ] ) {
            if ( it == end )
                _children[ c ]->_isEnd = true;
            else
                _children[ c ]->addWord( subword );
        }
        else {
            TrieUtf8 *tmp = new TrieUtf8( it == end );
            tmp->addWord( subword );
            _children[ c ] = tmp;
        }
    }
}



bool TrieUtf8::isPrefix( const std::string &pref ) const {
    Utf8Iterator it = pref.begin();
    Utf8Iterator end = pref.end();
    if ( it == end )
        return true;

    char32_t c = *it;
    ++it;
    if ( _children.find( c ) != _children.end() ) {
        return _children.find( c )->second
            ->isPrefix( it.substr( end ) );
    }
    return false;
}



bool TrieUtf8::isWord( const std::string &word ) const {
    if ( word.length() == 0 )
        return _isEnd;

    std::string cursub( word );
    const TrieUtf8 *tmp = this;

    while ( cursub.length() > 0 ) {
        Utf8Iterator it( cursub.begin() );
        Utf8Iterator end( cursub.end() );
        char32_t c = *it;
        ++it;
        if ( tmp->_children.find( c ) != tmp->_children.end() ) {
            tmp = tmp->_children.find( c )->second;
            cursub = it.substr( end );
        } else {
            return false;
        }
    }
    return tmp->_isEnd;
}



void TrieUtf8::getWords( WordSet &words, std::string wordSoFar ) const {
    if ( _isEnd ) {
        words.insert( wordSoFar );
    }
    for ( const auto &it : _children ) {
        std::string tmp = wordSoFar + CodePointToUtf8( it.first );
        if ( it.second && it.second->_isEnd ) {
            words.insert( tmp );
        }
        it.second->getWords( words, tmp );
    }
}



void TrieUtf8::getWordsStartingWith( const std::string &prefix, WordSet &words, std::string wordSoFar ) const {
    if ( prefix.size() == 0 ) {
        getWords( words, wordSoFar );
        return;
    }
    Utf8Iterator it = prefix.begin();
    Utf8Iterator end = prefix.end();
    char32_t c = *it;
    ++it;
    std::string subword = it.substr( end );
    if ( _children.find( c ) != _children.end() ) {
        TrieUtf8 *tmp = _children.find( c )->second;
        std::string nwsf = wordSoFar + CodePointToUtf8( c );
        tmp->getWordsStartingWith( subword, words, nwsf );
    }
}



std::string TrieUtf8::quoteMeta( const std::string &str ) const {
    static std::string special( "-.+*~$()[]\\|?" );

    std::string tmp;
    size_t last = 0;
    auto pos = str.find_first_of( special );

    while ( pos != std::string::npos ) {
        tmp += std::string( str.begin()+last, str.begin()+pos )
            + "\\" + str[pos];
        last = pos + 1;
        pos = str.find_first_of( special, pos+1 );
    }
    tmp += std::string( str.begin()+last, str.end() );

    return tmp;
}



std::string TrieUtf8::getRegexp() const {
    if ( _isEnd and _children.size() == 0 )
        return "";

    std::vector<std::string> alt;
    std::vector<std::string> cc;
    bool q = false;
    // map keys should already be sortted
    for ( auto &e : _children ) {
        //std::cout << CodePointToUtf8( e.first ) << ": " << *e.second;
        std::string qchar = quoteMeta( CodePointToUtf8( e.first ) );
        auto recurse = e.second->getRegexp();
        if ( recurse.length() > 0 )
            alt.push_back( qchar + recurse );
        else
            cc.push_back( qchar );

        if ( _isEnd and _children.size() > 0 )
            q = true;
    }
    bool cconly = alt.size() == 0;
    if ( cc.size() > 0 )
        alt.push_back(
            cc.size() == 1
            ? cc.front()
            : "[" + boost::algorithm::join( cc, "" ) + "]"
        );

    std::string result = ( alt.size() == 1 ) ? alt.front()
        : "(?:" + boost::algorithm::join( alt, "|" ) + ")";

    if ( q )
        result = cconly ? result + "?" : "(?:" + result + ")?";

    return result;
}



std::ostream &operator<<( std::ostream &ss, const TrieUtf8 &t ) {
    ss << t._isEnd << ", " << t._size << ", [";
    for ( const auto &e : t._children )
        ss << CodePointToUtf8( e.first ) << " ";
    ss << "]\n";
    return ss;
}



