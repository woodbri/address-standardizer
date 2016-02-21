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

#ifndef RULESECTION_H
#define RULESECTION_H

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

#include "rule.h"


class RuleSection {

public:
    RuleSection() {};
    RuleSection( const std::string &name ) : name_(name) {};
    std::string name() const { return name_; };
    long unsigned int size() const { return rules_.size(); };
    Rule rule( long unsigned int index ) const;
    std::vector<Rule> rules() const { return rules_; };

    void name( const std::string &name ) { name_ = name; };
    void rules( const std::vector<Rule> &rules ) { rules_ = rules; };
    void push_back( Rule &m ) { rules_.push_back( m ); };

    friend std::ostream &operator<<(std::ostream &ss, const RuleSection &rs);

private:
    std::string name_;
    std::vector<Rule> rules_;

};

#endif
