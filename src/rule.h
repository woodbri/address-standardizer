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

#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>

#include "inclass.h"
#include "outclass.h"

class Rule
{
public:

    // constructors
    Rule( const Rule &rule ) = default;
    Rule() : score_(0.0) {};
    explicit Rule( const std::string &line );

    // accessors
    std::vector<InClass::Type> in() const { return inClass_; };
    std::vector<OutClass::Type> out() const { return outClass_; };
    InClass::Type in(long unsigned int pos) const;
    OutClass::Type out(long unsigned int pos) const;
    float score() const { return score_; };
    long unsigned int inSize() const { return inClass_.size(); };
    long unsigned int outSize() const { return outClass_.size(); };
    bool isValid() const;
    bool isEmpty() const;

    // algorithms
    Rule concat( const Rule &r ) const;
    bool match( const std::vector<InClass::Type> pat ) const;

    // operators
    inline Rule operator+( const Rule &rhs ) const { return concat( rhs ); };
    friend std::ostream &operator<<(std::ostream &ss, const Rule &r);

    // setters
    void clear();

private:

    void pushIn( const InClass::Type t );
    void pushOut( const OutClass::Type t );
    void pushInOut( const InClass::Type in, const OutClass::Type out);
    void score( const float s );


/*
    // TODO these probably are not needed so made private
    // clean up later

    void in( const std::vector<InClass::Type> &in );
    void out( const std::vector<OutClass::Type>  &out );
*/

private:

    std::vector<InClass::Type> inClass_;
    std::vector<OutClass::Type> outClass_;
    float score_;

};

#endif
