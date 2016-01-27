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
#include "rule.h"
#include "grammar.h"


class Search : Grammar
{
public:

    Search( const Grammar &G ) : Grammar( G ), pos_(0) {};
    Search( const Grammar &G, const std::vector<InClass::Type> &pattern )
        : Grammar( G ),  pattern_(pattern), pos_(0) {};

    bool search();
    bool search( const std::vector<InClass::Type> &pattern );

    int numResults() const { return results_.size(); };
    std::vector<std::vector<Rule> > results() const { return results_; };
    std::vector<Rule> bestResult() const;

private:

    bool matchAllMeta( const Rule &rule, const int lvl );
    bool match( const std::string &name, const int lvl );
    bool match( const Rule &rule, const int lvl);

private:

    std::vector<InClass::Type> pattern_;
    int pos_;
    std::vector<std::vector<Rule> > results_;
    std::vector<Rule> stack_;

};

#endif
