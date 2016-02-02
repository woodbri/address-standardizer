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

#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "grammar.h"

Grammar::Grammar( const std::string &file )
    : issues_(""), status_(CHECK_OK)
{

    std::string line;
    std::ifstream is (file.c_str(), std::ifstream::in|std::ifstream::binary);
    if (not is.is_open()) {
        std::cerr << "ERROR: Failed to open file: '" << file << "'\n";
        //throw 2000
        return;
    }

    boost::regex re_comment("^\\s*#.*|^\\s*$");
    boost::regex re_section("^\\s*\\[(.+)\\]\\s*$");
    boost::regex re_ismeta("^(?:\\s*@(?:[\\w_])+)+\\s*$");

    std::string gotSection;
    std::vector<Rule> rules;

    boost::smatch what;
    int line_cnt = 0;
    while (not is.eof()) {
        ++line_cnt;
        std::getline( is, line );
        //std::cout << "\t" << line_cnt << ": " << line << "\n";

        // skip over comments and blank lines
        if ( boost::regex_match(line, re_comment, boost::match_default) ) 
            continue;
        // handle a section header
        else if ( boost::regex_match(line, what, re_section, boost::match_extra) ) {
            // if we already have section data, then save 
            if (gotSection.length() > 0) {
                rules_[gotSection] = rules;
                gotSection.clear();
                rules.clear();
            }
            gotSection = what[1];
        }
        // load data into section
        else {
            if (gotSection.length() == 0) {
                std::cerr << "ERROR: trying to load grammar section data outside of a section!\n";
                std::cerr << line_cnt << ": " << line << "\n";
                exit(1);
            }

            // check if this is a meta rule
            if ( line.find('@') != std::string::npos ) {
                // make sure all tokens are meta
                if (boost::regex_match(line, re_ismeta, boost::match_default) ) {
                    Rule rule( line, true );
                    rules.push_back( rule );
                }
                else {
                    // ERROR all names must match /@([\w_]+)/
                    std::cerr << "ERROR: all names must match pattern /@([\\w_]+)/ on meta rule lines!\n";
                    std::cerr << line_cnt << ": " << line << "\n";
                    exit(1);
                }
            }
            // otherwise it is a standard rule line
            else {
                Rule rule( line, false );
                rules.push_back( rule );
            }
        }
    }
    // save the last section with read in
    if (gotSection.length() > 0) {
        rules_[gotSection] = rules;
        gotSection.clear();
        rules.clear();
    }

    is.close();

    check();
    if ( status_ != 0 )
        std::cerr << "Grammar::Grammar(): " << issues_ << "\n";
}


void Grammar::check() {
    issues_.clear();
    references_.clear();
    status_ = CHECK_OK;

    check( "", "ADDRESS" );

    for ( const auto &e : rules_ ) {
        auto ref = references_.find( e.first );
        if ( ref == references_.end() and e.first != "ADDRESS" ) {
            issues_ += "Rule '" + e.first + "' defined by not referenced!\n";
            if ( status_ == CHECK_OK )
                status_ = CHECK_WARN;
        }
    }

    // done with reference vector so clear it
    references_.clear();
}


void Grammar::check( std::string section, std::string key ) {

    auto rule = rules_.find( key );
    if ( rule == rules_.end() ) {
        issues_ += "Missing rule: " + key +
            " : referenced at [" + section + "]\n";
        status_ = CHECK_FATAL;
        return;
    }
    for ( const auto &r : (*rule).second ) {
        if (r.isMeta()) {
            std::vector<std::string> words = r.meta();
            for ( const auto &word : words ) {
                check( key, word );
                auto ref = references_.find( word );
                if (ref == references_.end())
                    references_[word] = 1;
                else
                    references_[word] = references_[word] + 1;
            }
        }
        else {
            if ( not r.isValid() ) {
                issues_ += "Invalid rule at [" + key + "]\n";
                status_ = CHECK_FATAL;
            }
            if ( r.score() <= 0.0 ) {
                issues_ += "Score <= 0 at [" + key + "]\n";
                if ( not ( status_ == CHECK_FATAL ))
                    status_ = CHECK_WARN;
            }
        }
    }
}



std::ostream &operator<<(std::ostream &ss, const Grammar &g) {
    for ( const auto &e : g.rules_ ) {
        ss << "[" << e.first << "]\n";
        for ( const auto &r : e.second )
            ss << r << "\n";
        ss << "\n";
    }

    return ss;
}
