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

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

#include "inclass.h"
#include "outclass.h"
#include "token.h"
#include "utils.h"
#include "metasection.h"
#include "rulesection.h"
#include "grammartrie.h"

class Grammar
{
public:

    typedef enum {
        CHECK_FATAL = -1,
        CHECK_OK    =  0,
        CHECK_WARN  =  1
    } Status;

    Grammar( const Grammar& ) = default;
    explicit Grammar( const std::string &file );
    explicit Grammar( std::istream &is );

    unsigned long int recursionLimit() const { return recursion_limit_; };
    void recursionLimit( unsigned long int limit ) { recursion_limit_ = limit; };

    Rule* match( vInClass pattern ) { return trie_.match( pattern ); };
    Rule* match( std::vector<std::vector<Token> > phrases );

    Rule* searchAndReclassBest( const std::vector<std::vector<Token> > &phrases, float &score );

    Status status() const { return status_; };
    std::string issues() const { return issues_; } ;

    friend std::ostream &operator<<(std::ostream &ss, const Grammar &g);

private:

    void updatePointers();

    void initialize( std::istream &is );
    SectionPtr getSectionPtr( const std::string &name );
    void buildTrie( const std::string &name );
    void walk( const SectionPtr &sptr, std::vector<SectionPtr> next, std::vector<Rule*> pathSoFar, unsigned long int level );
    void addPathToTrie( std::vector<Rule*> path );

private:

    typedef enum {
        ISMETA,
        ISRULE,
        UNKNWN
    } RuleType;


protected:

    std::vector<MetaSection> metas_;
    std::vector<RuleSection> rules_;
    std::map<std::string,unsigned long int> sectionIndex_;
    GrammarTrie trie_;
    unsigned long int recursion_limit_;

    // temp storage for analysis and checking of grammar
    std::string issues_;
    std::map<std::string, int> references_;
    std::set<std::string> checked_;
    Status status_;

};

#endif
