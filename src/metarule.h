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

#ifndef METARULE_H
#define METARULE_H

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>


class MetaRule {

public:
    MetaRule() {};
    MetaRule( const std::string &line );
    long unsigned int size() const { return refs_.size(); };
    std::vector<std::string> refs() const { return refs_; };

    void refs( const std::vector<std::string> &refs ) { refs_ = refs; };
    void push_back( std::string &str ) { refs_.push_back( str ); };

    friend std::ostream &operator<<(std::ostream &ss, const MetaRule &r);

private:
    std::vector<std::string> refs_;

};

#endif
