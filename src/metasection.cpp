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


#include "metasection.h"

MetaRule MetaSection::rule( long unsigned int index ) const {
    if ( index < rules_.size() )
        return rules_[index];
    throw std::runtime_error("MetaSection-Range-Error");
}


std::ostream &operator<<(std::ostream &ss, const MetaSection &e) {
    ss << "[" << e.name() << "]\n";
    for ( const auto &r : e.rules() ) 
        ss << r << "\n";
    return ss;
}
