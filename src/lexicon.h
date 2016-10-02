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

#ifndef LEXICON_H
#define LEXICON_H

#include <stdexcept>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/regex/icu.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "trieutf8.h"
#include "inclass.h"
#include "token.h"
#include "lexentry.h"

class Lexicon
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & name_;
        ar & lang_;
        ar & locale_;
        ar & lex_;
        ar & regex_;
        ar & regexPrefix_;
        ar & regexSuffix_;
    }

protected:

public:
    // constructors
    Lexicon();
    explicit Lexicon(std::string name);
    Lexicon(std::string name, std::string file);
    Lexicon(std::string name, std::istream &is);

    void initialize(std::istream &is);

    // getters
    std::string name() const { return name_; };
    InClass::Lang lang() const { return lang_; };
    std::string langAsString() const { return InClass::asString(lang_); };
    std::string langAsName() const { return InClass::asName(lang_); };
    std::string locale() const { return locale_; };

    std::vector<LexEntry> find( const std::string key );

    // mutators
    void name( const std::string name ) { name_ = name; };
    void lang( const InClass::Lang lang ) { lang_ = lang; };
    void lang( const std::string lang ) { lang_ = InClass::asLang( lang ); };
    void locale( const std::string locale ) { locale_ = locale; };

    void insert( const LexEntry &le );
    void remove( const LexEntry &le );

    std::string regex();
    std::string regexPrefixAtt();
    std::string regexSuffixAtt();

    // operators
    friend std::ostream &operator<<(std::ostream &ss, const Lexicon &lex);

    // algorithms
    void classify( Token& token, InClass::Type typ );
    void standardize( Token& token );

private:

    std::string escapeRegex( const std::string &str);

    struct lexcomp {
        bool operator() (const std::string &lhs, const std::string &rhs) const {
            return lhs<rhs;
        }
    };


private:
    std::string name_;
    InClass::Lang lang_;
    std::string locale_;
    std::map <std::string, std::vector<LexEntry>, lexcomp> lex_;
    std::string regex_;
    std::string regexPrefix_;
    std::string regexSuffix_;


};

#endif
