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

#include <iostream>
#include <sstream>

#include "token.h"

Token::Token() : outclass_(OutClass::BADTOKEN) {
    text_.clear();
    stdtext_.clear();
    inclass_.clear();
    attached_.clear();
    inlex_ = false;
}


Token::Token(std::string text) {
    std::string in_word;
    std::string in_stdword;
    std::string in_inclass;
    std::string in_outclass;
    std::string in_attached;

    std::stringstream buffer(text);
    std::getline(buffer, in_word, '\t');
    if (in_word == "TOKEN:")
        std::getline(buffer, in_word, '\t');
    std::getline(buffer, in_stdword, '\t');
    std::getline(buffer, in_inclass, '\t');
    std::getline(buffer, in_outclass, '\t');
    std::getline(buffer, in_attached, '\t');

    text_ = in_word;
    stdtext_ = in_stdword;
    inclass_ = InClass::asType(in_inclass);
    outclass_ = OutClass::asType(in_outclass);
    attached_ = InClass::asAttachType(in_attached);
    inlex_ = false;

}


std::ostream &operator<<(std::ostream &ss, const Token &token) {
    std::string inclass = token.inclassAsString();
    std::string attached = token.attachedAsString();
    std::string outclass = token.outclassAsString();

    ss << "TOKEN:" << "\t"
       << token.text_ << "\t"
       << token.stdtext_ << "\t"
       << inclass << "\t"
       << outclass << "\t"
       << attached << "\t"
       << token.inlex_;

    return ss;
}


bool Token::isInClass( InClass::Type test ) const {
    if (inclass_.find( test ) != inclass_.end())
        return true;
    return false;
}


bool Token::isInClass( std::set<InClass::Type> test ) const {
    for ( const auto &e : test ) {
        if (inclass_.find( e ) != inclass_.end())
            return true;
    }
    return false;
}


InClass::Type Token::in(long unsigned int pos) const {
    auto it = inclass_.begin();
    for (long unsigned int i=0; i<pos; ++i)
        it++;
    return *it;
}


std::vector< std::vector<InClass::Type> > Token::enumerate( std::vector<Token> tokens ) {

    // count the number of possible combinations
    long unsigned int cnt = 1;
    for (const auto &t : tokens)
        cnt *= t.inSize();

    // reserve space for all the combinations
    std::vector< std::vector<InClass::Type> > list;
    list.clear();
    list.reserve( list.size() );

    // enumerate all the combinations and save them in list
    for (long unsigned int i=0; i<cnt; ++i) {
        long unsigned int num = 1;
        std::vector<InClass::Type> one;
        for ( const auto &t : tokens ) {
            num *= t.inSize();
            one.push_back( t.in( i*num/cnt % t.inSize() ) );
            //std::cout << InClass::asString(one.back()) << " ";
        }
        list.push_back( one );
        //std::cout << "\n";
    }

    return list;
}
