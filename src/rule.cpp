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

#include <sstream>
#include <boost/regex.hpp>
#include <stdexcept>

#include "rule.h"


Rule::Rule( const std::string &line ) : score_(0.0) {
    std::string token;
    std::stringstream buffer(line);

    // else read normal rule here
    int state = 0;
    while (not buffer.eof()) {
        buffer >> token;

        if (token == "->" ) {
            ++state;
            continue;
        }

        switch (state) {
            case 0:     // reading inClass tokens
                pushIn( InClass::asOneType( token ) );
                break;
            case 1:     // reading outClass tokens
                pushOut( OutClass::asType( token ) );
                break;
            case 2:     // reading score
                score( static_cast<float>( atof( token.c_str() ) ) );
                break;
        }
    }

    if (not isValid()) {
        throw std::runtime_error("Rule-Not-Valid("+line+")");
    }
}


InClass::Type Rule::in(long unsigned int pos) const {
    if ( pos < inClass_.size() )
        return inClass_[pos];
    return InClass::BADTOKEN;
}


OutClass::Type Rule::out(long unsigned int pos) const {
    if ( pos < outClass_.size() )
        return outClass_[pos];
    return OutClass::BADTOKEN;
}


bool Rule::isEmpty() const {
    return inClass_.size()==0 and outClass_.size()==0 and score_==0;
}

bool Rule::isValid() const {
    if ( inClass_.size() != outClass_.size() or score_ <= 0 )
        return false;

    return true;
}


// algorithms

Rule Rule::concat( const Rule &r ) const {
    Rule conc( *this );

    // TODO decide how to deal with common tokens across
    //      concatenation boundary like:
    //      ... WORD | WORD ...

    // append r to conc
    conc.inClass_.insert( conc.inClass_.end(), r.inClass_.begin(), r.inClass_.end() );
    conc.outClass_.insert( conc.outClass_.end(), r.outClass_.begin(), r.outClass_.end() );

    conc.score_ = static_cast<float>( (score_ + r.score()) / 2.0 );

    return conc;
}

// see if we can match the start of pattern to this rule
// if we run out of tokens in pattern then it fails to match

bool Rule::match( const std::vector<InClass::Type> &pat ) const {
    if ( pat.size() < inClass_.size() )
        return false;

    auto it = pat.begin();
    for (auto e = inClass_.begin(); e != inClass_.end(); ++e ) {
        if ( *e != *it )
            return false;
        ++it;
    }
    return true;
}


// setters

void Rule::clear() {
    inClass_.clear();
    outClass_.clear();
    score_ = 0.0;
}


void Rule::pushIn( const InClass::Type t ) {
    inClass_.push_back(t);
}


void Rule::pushOut( const OutClass::Type t ) {
    outClass_.push_back(t);
}


void Rule::pushInOut(const InClass::Type in, const OutClass::Type out) {
    inClass_.push_back(in);
    outClass_.push_back(out);
}


void Rule::score( const float s ) {
    score_ = s;
}


std::ostream &operator<<(std::ostream &ss, const Rule &r) {
    std::string word;

    for ( long unsigned int pos = 0; pos < r.inSize(); ++pos )
        ss << InClass::asString( r.in( pos ) ) << " ";
    ss << "-> ";
    for ( long unsigned int pos = 0; pos < r.outSize(); ++pos )
        ss << OutClass::asString( r.out( pos ) ) << " ";
    ss << "-> " << r.score();

    return ss;
}


