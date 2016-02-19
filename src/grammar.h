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
#include "metasection.h"
#include "rulesection.h"

class Grammar
{
public:

    typedef enum {
        CHECK_FATAL = -1,
        CHECK_OK    =  0,
        CHECK_WARN  =  1
    } Status;

    Grammar( const Grammar& ) = default;
    Grammar( const std::string &file );
    Grammar( std::istream &is );

    void initialize( std::istream &is );
    void check();
    void check( std::string section, std::string key );
    Status status() const { return status_; };
    std::string issues() const { return issues_; } ;

    friend std::ostream &operator<<(std::ostream &ss, const Grammar &g);


private:

    typedef enum {
        ISMETA,
        ISRULE,
        UNKNWN
    } RuleType;


protected:

    std::map<std::string, MetaSection> metas_;
    std::map<std::string, RuleSection> rules_;

    // temp storage for analysis and checking of grammar
    std::string issues_;
    std::map<std::string, int> references_;
    Status status_;

};

#endif
