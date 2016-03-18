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
#include <boost/regex/icu.hpp>
#include <boost/lexical_cast.hpp>

#include "grammar.h"

Grammar::Grammar( const std::string &file )
    : issues_(""), status_(CHECK_OK)
{
    std::ifstream ifs;
    ifs.open( file.c_str(), std::ifstream::in);
    initialize( ifs );
    ifs.close();
}


Grammar::Grammar( std::istream &is )
    : issues_(""), status_(CHECK_OK)
{
    initialize( is );
}


void Grammar::initialize( std::istream &is )
{

    auto re_comment = boost::make_u32regex( "^\\s*#.*|^\\s*$" );
    auto re_section = boost::make_u32regex( "^\\s*\\[(.+)\\]\\s*$" );
    auto re_ismeta  = boost::make_u32regex( "^(?:\\s*@(?:[\\w_])+)+\\s*$" );

    std::string gotSection;
    RuleType ruleType = UNKNWN;
    std::vector<MetaRule> mrules;
    std::vector<Rule> rules;
    std::string line;

    boost::smatch what;
    int line_cnt = 0;
    while ( is ) {
        ++line_cnt;
        std::getline( is, line );
        //std::cout << "\t" << line_cnt << ": " << line << "\n";

        // remove UTF8 BOM if one exists
        if ( line_cnt == 1 and line[0] == '\xEF' and line[1] == '\xBB' and line[2] == '\xBF' )
            line = line.substr(4);

        // skip over comments and blank lines
        if ( boost::u32regex_match( line, re_comment ) ) 
            continue;
        // handle a section header
        else if ( boost::u32regex_match( line, what, re_section, boost::regex_constants::match_extra ) ) {
            // the current data if any
            if ( ruleType != UNKNWN and gotSection == "") {
                    throw std::runtime_error("Grammar-Empty-Section-Defined");
            }
            else if ( ruleType == ISMETA ) {
                MetaSection ms(gotSection);
                ms.rules(mrules);
                metas_[gotSection] = ms;
                ruleType = UNKNWN;
                mrules.clear();
                rules.clear();
            }
            else if ( ruleType == ISRULE ) {
                RuleSection rs(gotSection);
                rs.rules(rules);
                rules_[gotSection] = rs;
                ruleType = UNKNWN;
                mrules.clear();
                rules.clear();
            }

            // start a new section
            gotSection = what[1];
    
            // check if the section already exists
            if ( metas_.find(gotSection) != metas_.end() or
                 rules_.find(gotSection) != rules_.end() )
                throw std::runtime_error("Grammar-Duplicate-Section_Name");
        }
        // load data into section
        else {
            if (gotSection.length() == 0)
                throw std::runtime_error("Grammar-Syntax-Error");

            // check if this is a meta rule
            if ( line.find('@') != std::string::npos ) {
                if ( ruleType == ISRULE )
                    throw std::runtime_error("Grammar-Mixing-Rules-and-Metas");

                ruleType = ISMETA;

                // make sure all tokens are meta
                if (boost::u32regex_match( line, re_ismeta ) ) {
                    MetaRule rule( line );
                    mrules.push_back( rule );
                }
            }
            // otherwise it is a standard rule line
            else {
                if ( ruleType == ISMETA )
                    throw std::runtime_error("Grammar-Mixing-Rules-and-Metas");

                ruleType = ISRULE;

                Rule rule( line );
                rules.push_back( rule );
            }
        }
    }
    // save the last section with read in
    if (gotSection.length() > 0) {
        if ( ruleType == ISMETA ) {
            MetaSection ms(gotSection);
            ms.rules(mrules);
            metas_[gotSection] = ms;
        }
        else if ( ruleType == ISRULE ) {
            RuleSection rs(gotSection);
            rs.rules(rules);
            rules_[gotSection] = rs;
        }

        ruleType = UNKNWN;
        mrules.clear();
        rules.clear();
    }

    updatePointers();

//    if ( status_ != 0 )
//        std::cerr << "Grammar::Grammar(): " << issues_ << "\n";

    if ( status_ == CHECK_FATAL )
        throw std::runtime_error( issues_ );

}


/*
void Grammar::check() {
    issues_.clear();
    references_.clear();
    checked_.clear();
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
    auto meta = metas_.find( key );

    // mark this key as checked
    checked_.insert( key );

    if ( rule != rules_.end() ) {
        for ( const auto &r : ((*rule).second).rules() ) {
            if ( not r.isValid() ) {
                issues_ += "Invalid rule at [" + key + "]\n";
                status_ = CHECK_FATAL;
            }
        }
    }
    else if ( meta != metas_.end() ) {
        for ( const auto &r : ((*meta).second).rules() ) {
            std::vector<std::string> words = r.refs();
            for ( const auto &word : words ) {
                // avoid recursive checking of already checked items
                if ( checked_.find( word ) == checked_.end() )
                    check( key, word );
                auto ref = references_.find( word );
                if (ref == references_.end())
                    references_[word] = 1;
                else
                    references_[word] = references_[word] + 1;
            }
        }
    }
    else {
        issues_ += "Missing rule: " + key +
            " : referenced at [" + section + "]\n";
        status_ = CHECK_FATAL;
        return;
    }
}
*/


void Grammar::updatePointers() {
    issues_.clear();
    references_.clear();
    status_ = CHECK_OK;
    for ( auto &meta : metas_ ) {
        std::string metaName = meta.second.name();
        for ( const auto &rules : meta.second.rules() ) {
            for ( auto &ref : rules.refs() ) {
                std::string word = ref.name();
                if ( references_.find( word ) == references_.end() )
                    references_[word] = 1;
                else
                    references_[word] = references_[word] + 1;

                if ( ref.name() == metaName )
                    ref.mptr( &( meta.second ) );
                else {
                    auto m = metas_.find( ref.name() );
                    if ( m != metas_.end() )
                        ref.mptr( &( m->second ) );
                    else {
                        auto r = rules_.find( ref.name() );
                        if ( r != rules_.end() ) {
                            ref.rptr( &( r->second ) );
                            for ( const auto &a : r->second.rules() ) {
                                if ( not a.isValid() ) {
                                    issues_ += "Invalid rule at [" + ref.name() + "]\n";
                                    status_ = CHECK_FATAL;
                                }
                            }

                        }
                        else {
                            issues_ += "Missing rule: " + ref.name() +
                                " : referenced at [" + metaName + "]\n";
                            status_ = CHECK_FATAL;
                        }
                    }
                }
            }
        }
    }

    // check the status of all references and make sure we have 'ADDRESS'

    const auto addr = metas_.find( "ADDRESS" );
    if ( addr == metas_.end() ) {
        issues_ += "Rule 'ADDRESS' is not defined!\n";
        status_ = CHECK_FATAL;
    }

    for ( const auto &e : metas_ ) {
        auto ref = references_.find( e.first );
        if ( ref == references_.end() and e.first != "ADDRESS" ) {
            issues_ += "Rule '" + e.first + "' defined by not referenced!\n";
            if ( status_ == CHECK_OK )
                status_ = CHECK_WARN;
        }
    }

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



std::ostream &operator<<(std::ostream &ss, const Grammar &g) {
    for ( const auto &e : g.metas_ ) {
//        ss << "[" << e.first << "]\n";
        ss << e.second << "\n";
//        ss << "\n";
    }
    for ( const auto &e : g.rules_ ) {
//        ss << "[" << e.first << "]\n";
        ss << e.second << "\n";
//        ss << "\n";
    }

    return ss;
}
