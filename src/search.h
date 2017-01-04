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

#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>

#include "inclass.h"
#include "outclass.h"
#include "token.h"
#include "rule.h"
#include "utils.h"
#include "sectionptr.h"
#include "grammar.h"

class SearchPath {
public:
    std::vector<Rule> rules;
    std::vector<SectionPtr> next;
    std::vector<InClass::Type> remaining;
};

typedef std::vector<SearchPath> SearchPaths;


class MatchResult {
public:
    std::string matched;
    double score;
    double nrules;
};

typedef std::vector<MatchResult> MatchResults;


class Search : Grammar
{
public:

    Search( const Grammar &G ) : Grammar( G ), recursion_limit_(20) {};

    SearchPaths search( const std::vector<Token> &phrase );

    SearchPaths search( const std::string &grammarNode, const std::vector<Token> &phrase );

    bool reclassTokens( std::vector<Token> &tokens, const SearchPath &path ) const;

    std::vector<Token> searchAndReclassBest( const std::vector<Token> &phrase, float &cost, std::string &matched, float &nrules );

    std::vector<Token> searchAndReclassBest( const std::vector<std::vector<Token> > &phrases, float &cost, std::string &matched, float &nrules );

    MatchResults searchAndReclassAll( const std::vector<std::vector<Token> > &phrases );

private:

    std::string toString( const std::vector<Token> &results ) const;
    SectionPtr stringToSectionPtr( std::string str );
    SearchPaths match( const SectionPtr &ptr, const SearchPaths &paths, const unsigned long int level ) const;
    SearchPaths match( const SectionPtr &ptr, const SearchPath &path, const unsigned long int level ) const;
    SearchPaths matchNext( const SearchPaths &paths, const unsigned long int level ) const;
    SearchPaths matchNext( const SearchPath &path, const unsigned long int level ) const;
    SearchPath matchRule( const Rule &r, const SearchPath &path ) const;

protected:
    unsigned long int recursion_limit_;

};

#endif
