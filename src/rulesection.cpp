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


#include "rulesection.h"

Rule RuleSection::rule( long unsigned int index ) const {
    if ( index < rules_.size() )
        return rules_[index];
    throw std::runtime_error("RuleSection-Range-Error");
}


std::ostream &operator<<(std::ostream &ss, const RuleSection &e) {
    ss << "[" << e.name() << "]\n";
    for ( const auto &r : e.rules() ) 
        ss << r << "\n";
    return ss;
}
