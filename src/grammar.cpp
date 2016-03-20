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
                sectionIndex_[gotSection] = metas_.size();
                metas_.push_back( ms );
                ruleType = UNKNWN;
                mrules.clear();
                rules.clear();
            }
            else if ( ruleType == ISRULE ) {
                RuleSection rs(gotSection);
                rs.rules(rules);
                sectionIndex_[gotSection] = rules_.size();
                rules_.push_back( rs );
                ruleType = UNKNWN;
                mrules.clear();
                rules.clear();
            }

            // start a new section
            gotSection = what[1];
    
            // check if the section already exists
            if ( sectionIndex_.find(gotSection) != sectionIndex_.end() )
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
    // save the last section we read in
    if (gotSection.length() > 0) {
        if ( ruleType == ISMETA ) {
            MetaSection ms(gotSection);
            ms.rules(mrules);
            sectionIndex_[gotSection] = metas_.size();
            metas_.push_back( ms );
        }
        else if ( ruleType == ISRULE ) {
            RuleSection rs(gotSection);
            rs.rules(rules);
            sectionIndex_[gotSection] = rules_.size();
            rules_.push_back( rs );
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


void Grammar::updatePointers() {
    issues_.clear();
    references_.clear();
    status_ = CHECK_OK;

    // for each MetaSection
    for ( unsigned long int i=0; i<metas_.size(); i++ ) {
        // get the name and a copy of this MetaSections rules
        const std::string& metaName = metas_[i].name();
        auto metaRules = metas_[i].rules();

        // for each metaRules
        for ( unsigned long int j=0; j<metaRules.size(); j++ ) {
            // get a copy of this metaRule SectionPtrs as refs
            auto refs = metaRules[j].refs();

            // for each SectionPtr in refs
            for ( auto &ref : refs ) {
                // get the name of this SectionPtr
                const std::string word = ref.name();

                // update the reference stats
                if ( references_.find( word ) == references_.end() )
                    references_[word] = 1;
                else
                    references_[word] = references_[word] + 1;

                if ( word == metaName )
                    // if this is self referential then point to self
                    ref.mptr( &( metas_[i] ) );
                else {
                    // otherwise find the SectionPtr this refers to
                    auto m = sectionIndex_.find( word );
                    if ( m != sectionIndex_.end() ) {
                        // see if it refers to a metaSection
                        if ( m->second < metas_.size()
                             and metas_[m->second].name() == word ) {
                            ref.mptr( &( metas_[m->second] ) );
                        }
                        // see if it refers to a ruleSection
                        else if ( m->second < rules_.size()
                                  and rules_[m->second].name() == word ) {
                            ref.rptr( &( rules_[m->second] ) );
                        }
                        // otherwise something really bad is happening
                        else
                            throw std::runtime_error( "Grammar-updatePointers-consistency-check-failed" );
                    }
                    // or flag it as an issue if it is missing
                    else {
                        issues_ += "Missing rule: " + word +
                            " : referenced at [" + metaName + "]\n";
                        status_ = CHECK_FATAL;
                    }
                }
            }
            // now that we have updated our local copy of refs
            // save it back into the appropriate metaRule
            metaRules[j].refs( refs );
        }
        // now that all the metaRules have been updated in our local copy
        // save them back into the original metaSection
        metas_[i].rules( metaRules );
    }

    // check the status of all references and make sure we have 'ADDRESS'

    const auto addr = sectionIndex_.find( "ADDRESS" );
    if ( addr == sectionIndex_.end() ) {
        issues_ += "Rule 'ADDRESS' is not defined!\n";
        status_ = CHECK_FATAL;
    }

    for ( const auto &e : metas_ ) {
        auto ref = references_.find( e.name() );
        if ( ref == references_.end() and e.name() != "ADDRESS" ) {
            issues_ += "Rule '" + e.name() + "' defined by not referenced!\n";
            if ( status_ == CHECK_OK )
                status_ = CHECK_WARN;
        }

        // check all the pointers are set correctly

        for ( const auto &rules : e.rules() ) {
            for ( const auto &r : rules.refs() ) {
                if ( r.mptr() == NULL and r.rptr() == NULL )
                    std::cout << "POINTERS: " << e.name() << "."
                        << r.name() << " both pointers are NULL!\n";
                if ( r.rptr() != NULL and r.mptr() != NULL )
                    std::cout << "POINTERS: " << e.name() << "."
                        << r.name() << " both are not NULL!\n";
            }
        }
    }

    // check all the rules and make sure they are valid

    for ( const auto &e : rules_ ) {
        auto ref = references_.find( e.name() );
        if ( ref == references_.end() and e.name() != "ADDRESS" ) {
            issues_ += "Rule '" + e.name() + "' defined by not referenced!\n";
            if ( status_ == CHECK_OK )
                status_ = CHECK_WARN;
        }
        if ( checked_.find( e.name() ) == checked_.end() ) {
            int pos = 1;
            for ( const auto &rule : e.rules() ) {
                if ( not rule.isValid() ) {
                    issues_ += "Invalid rule at [" + e.name() + "]["
                        + std::to_string(pos) + "]\n";
                    status_ = CHECK_FATAL;
                }
                ++pos;
            }
            checked_.insert( e.name() );
        }
    }

    // done with reference_ and checked_ so clear them
    references_.clear();
    checked_.clear();
}



std::ostream &operator<<(std::ostream &ss, const Grammar &g) {
    for ( const auto &e : g.metas_ ) {
        ss << e << "\n";
    }
    for ( const auto &e : g.rules_ ) {
        ss << e << "\n";
    }

    return ss;
}



