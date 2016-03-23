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
// And getRegexp() from Perl Regexp::Trie module

#include "trie.h"


void Trie::addWord( std::string word ) {
    if ( word.length() > 0 ) {
        // counts number of words added
        // but if you add the same word more than once
        // this size will differ from the set size
        // which is unique words in trie
        ++_size;
        std::string subword = word.substr( 1, word.size()-1 );
        if ( _children[ word[0] ] ) {
            if ( word.size() == 1 )
                _children[ word[0] ]->_isEnd = true;
            else
                _children[ word[0] ]->addWord( subword );
        } else {
            Trie *tmp = new Trie( word.size() == 1 );
            tmp->addWord( subword );
            _children[ word[0] ] = tmp;
        }
    }
}



bool Trie::isPrefix( const std::string &pref ) const {
    if ( pref.length()== 0 ) {
        return true;
    }
    if ( _children.find( pref[0] ) != _children.end() ) {
        return _children.find( pref[0] )->second
            ->isPrefix( pref.substr( 1, pref.size()-1 ) );
    }
    return false;
}



bool Trie::isWord( const std::string &word ) const {
    if ( word.length() == 0 ) {
        return _isEnd;
    }
    std::string cursub( word );
    const Trie *tmp = this;

    while ( cursub.length() > 0 ) {
        if ( tmp->_children.find( cursub[0] ) != tmp->_children.end() ) {
            tmp = tmp->_children.find( cursub[0] )->second;
            cursub = cursub.substr( 1, cursub.size()-1 );
        } else {
            return false;
        }
    }
    return tmp->_isEnd;
}



void Trie::getWords( WordSet &words, std::string wordSoFar ) const {
    if ( _isEnd ) {
        words.insert( wordSoFar );
    }
    for ( const auto &it : _children ) {
        std::string tmp = wordSoFar + std::string( 1, it.first );
        if ( it.second && it.second->_isEnd ) {
            words.insert( tmp );
        }
        it.second->getWords( words, tmp );
    }
}



void Trie::getWordsStartingWith( std::string prefix, WordSet &words, std::string wordSoFar ) const {
    if ( prefix.size() == 0 ) {
        getWords( words, wordSoFar );
        return;
    }
    std::string subword = prefix.substr( 1, prefix.size()-1 );
    if ( _children.find( prefix[0] ) != _children.end() ) {
        Trie *tmp = _children.find( prefix[0] )->second;
        std::string nwsf = wordSoFar + std::string( 1, prefix[0] );
        tmp->getWordsStartingWith( subword, words, nwsf );
    }
}



std::string Trie::quoteChar( const char c ) const {
    static std::string special( "-.+*~$()[]\\|?" );
    auto found = special.find_first_of( c );
    if ( found == std::string::npos )
        return std::string( 1, c );
    else
        return "\\" + c;
}



std::string Trie::getRegexp() const {
    if ( _isEnd and _children.size() == 0 )
        return "";

    std::vector<std::string> alt;
    std::vector<std::string> cc;
    bool q = false;
    // map keys should already be sortted
    for ( auto &e : _children ) {
        //std::cout << e.first << ": " << *e.second;
        std::string qchar = quoteChar( e.first );
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



std::ostream &operator<<( std::ostream &ss, const Trie &t ) {
    ss << t._isEnd << ", " << t._size << ", [";
    for ( const auto &e : t._children )
        ss << e.first << " ";
    ss << "]\n";
    return ss;
}



