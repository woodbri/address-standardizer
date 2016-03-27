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

#ifndef METASECTION_H
#define METASECTION_H

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

#include "metarule.h"


class MetaSection {

public:
    MetaSection() {};
    explicit MetaSection( const std::string &name ) : name_(name) {};
    std::string name() const { return name_; };
    long unsigned int size() const { return rules_.size(); };
    MetaRule rule( long unsigned int index ) const;
    std::vector<MetaRule> rules() const { return rules_; };

    void name( const std::string &name ) { name_ = name; };
    void rules( const std::vector<MetaRule> &rules ) { rules_ = rules; };
    void push_back( MetaRule &m ) { rules_.push_back( m ); };

    friend std::ostream &operator<<(std::ostream &ss, const MetaSection &ms);

    // iterator access to rules_

    typedef typename std::vector<MetaRule>::iterator iterator;
    typedef typename std::vector<MetaRule>::const_iterator const_iterator;

    iterator begin() { return rules_.begin(); };
    iterator end() { return rules_.end(); };
    const_iterator begin() const { return rules_.begin(); };
    const_iterator end() const { return rules_.end(); };



private:
    std::string name_;
    std::vector<MetaRule> rules_;

};

#endif
