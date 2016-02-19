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
#include "grammar.h"

class MatchResult {
public:
    std::vector<Rule> rules;
    std::vector<InClass::Type> remaining;
};

typedef std::vector<MatchResult> MatchResults;



class Search : Grammar
{
public:

    Search( const Grammar &G ) : Grammar( G ) {};

    MatchResults search( const std::vector<Token> &phrase ) const;

    MatchResults search( const std::string &grammarNode, const std::vector<Token> &phrase ) const;

    bool reclassTokens(std::vector<Token> &tokens, const MatchResult &result ) const;

    MatchResult searchAndReclassBest( std::vector<Token> &phrase, float &cost ) const;

    //MatchResult searchAndReclassBest( std::vector<std::vector<Token> > &phrases, float &cost ) const;

private:

    MatchResults match( const std::string &name, const MatchResults &results ) const;
    MatchResults match( const std::string &name, const MatchResult &result ) const;
    MatchResults matchMeta( const MetaRule &r, const MatchResults &results ) const;
    MatchResults matchMeta( const MetaRule &r, const MatchResult &result ) const;
    MatchResults mergeResults( const MatchResult &orig, const MatchResult &added ) const;
    MatchResults mergeResults( const MatchResult &orig, const MatchResults &added ) const;
    MatchResult matchRule( const Rule &r, const std::vector<InClass::Type> pattern ) const;


};

#endif
