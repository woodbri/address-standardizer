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

#include "inclass.h"
#include "outclass.h"
#include "rule.h"

class Grammar
{
public:
/*
    typedef struct {
        std::vector<InClass::Type> inKey_;
    } Key;

    typedef struct {
        std::vector<OutClass::Type> outKey_;
        float score_;
    } Value;
*/

    Grammar( const std::string &file );

    std::string check() const;

    friend std::ostream &operator<<(std::ostream &ss, const Grammar &g);


protected:

    std::map<std::string, std::vector<Rule> > rules_;


};

#endif
