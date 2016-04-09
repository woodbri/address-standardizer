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

//#define DEBUG
//#define DEBUG_TRIE
//#define DEBUG_WALK
#define DEBUG_COUNTS

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


Rule* Grammar::searchAndReclassBest( const std::vector<std::vector<Token> > &phrases, float &score ) {

    Rule* best;
    float bestScore = -1.;

    for ( auto &phrase : phrases ) {
        auto patterns = Token::enumerate( phrase );
        for ( const auto &pattern : patterns ) {
            float thisScore;
            auto result = match( pattern );
            if ( result == NULL )
                continue;
            thisScore = result->score();
            if ( thisScore > bestScore ) {
                bestScore = thisScore;
                best = result;
            }
        }
    }

    score = bestScore;
    return best;
}


void Grammar::initialize( std::istream &is )
{
    recursion_limit_ = 100;

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

    buildTrie( std::string("ADDRESS") );

#ifdef DEBUG_COUNTS
    std::cout << "addPathToTrie: counts: "
              << Utils::getCount( "addPathToTrie" ) << "\n";
#endif

#ifdef DEBUG_TRIE
     PatternSet dbg_patterns;
     trie_.getPatterns( dbg_patterns );
     for ( auto it=dbg_patterns.begin(); it != dbg_patterns.end(); ++it ) {
        std::cout << "GTrie: ";
        for ( const auto &p : *it )
            std::cout << InClass::asString( p ) << " ";
        std::cout << "\n";
    }
#endif

}


void Grammar::updatePointers() {
    issues_.clear();
    references_.clear();
    status_ = CHECK_OK;

    // for each MetaSection
    for ( auto meta = metas_.begin(); meta != metas_.end(); ++meta ) {
        const std::string& metaName = meta->name();

        // for each metaRules
        for ( auto refs = meta->begin(); refs != meta->end(); ++refs ) {

            // for each SectionPtr in refs
            for ( auto ref = refs->begin(); ref != refs->end(); ++ref ) {
                // get the name of this SectionPtr
                const std::string word = ref->name();

                // update the reference stats
                if ( references_.find( word ) == references_.end() )
                    references_[word] = 1;
                else
                    references_[word] = references_[word] + 1;

                if ( word == metaName ) {
                    // if this is self referential then point to self
                    issues_ += "Recursive rule: " + word + "\n";
                    status_ = CHECK_FATAL;
                }
                else {
                    // otherwise find the SectionPtr this refers to
                    auto m = sectionIndex_.find( word );
                    if ( m != sectionIndex_.end() ) {
                        // see if it refers to a metaSection
                        if ( m->second < metas_.size()
                             and metas_[m->second].name() == word ) {
                            ref->mptr( &( metas_[m->second] ) );
                        }
                        // see if it refers to a ruleSection
                        else if ( m->second < rules_.size()
                                  and rules_[m->second].name() == word ) {
                            ref->rptr( &( rules_[m->second] ) );
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
        }
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

#ifdef DEBUG

        // check all the pointers are set correctly
        for ( auto rules = e.begin(); rules != e.end(); ++rules ) {
            for ( auto r = rules->begin(); r != rules->end(); ++r ) {
                if ( r->mptr() == NULL and r->rptr() == NULL )
                    std::cout << "POINTERS: " << e.name() << "."
                        << r->name() << " both pointers are NULL!\n";
                if ( r->rptr() != NULL and r->mptr() != NULL )
                    std::cout << "POINTERS: " << e.name() << "."
                        << r->name() << " both are not NULL!\n";
            }
        }
#endif
    }

    // check all the rules and make sure they are valid

    for ( const auto e : rules_ ) {
        auto ref = references_.find( e.name() );
        if ( ref == references_.end() and e.name() != "ADDRESS" ) {
            issues_ += "Rule '" + e.name() + "' defined by not referenced!\n";
            if ( status_ == CHECK_OK )
                status_ = CHECK_WARN;
        }
        if ( checked_.find( e.name() ) == checked_.end() ) {
            int pos = 1;
            for ( auto rule = e.begin(); rule != e.end(); ++rule ) {
                if ( not rule->isValid() ) {
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



SectionPtr Grammar::getSectionPtr( const std::string &str ) {
    auto idx = sectionIndex_.find( str );
    if ( idx != sectionIndex_.end() ) {
    
        if ( idx->second < metas_.size()
             and str == metas_[idx->second].name() ) {
            SectionPtr ptr( str );
            ptr.mptr( & metas_[idx->second] );
            return ptr;
        }

        if ( idx->second < rules_.size()
             and str == rules_[idx->second].name() ) {
            SectionPtr ptr( str );
            ptr.rptr( & rules_[idx->second] );
            return ptr;
        }

    }
    throw std::runtime_error( std::string("Grammar-Rule-Not-Found:")+str );
}



void Grammar::buildTrie( const std::string &name ) {
    auto ptr = getSectionPtr( name );
    walk( ptr, std::vector<SectionPtr>(), std::vector<Rule*>(), 0 );
}


void Grammar::walk( const SectionPtr &sptr, std::vector<SectionPtr> next, std::vector<Rule*> pathSoFar, unsigned long int level ) {

#ifdef DEBUG_WALK
    std::cout << "Grammar::walk(" << sptr << ", (";
    for ( const auto &n : next )
        std::cout << n << ", ";
    std::cout << "), (";
    for ( const auto &r : pathSoFar )
        for ( auto it = r->begin(); it != r->end(); ++it )
            std::cout << InClass::asString( *it ) << " ";
    std::cout << "), " << level <<")\n";
#endif

    if ( level > recursion_limit_ ) {
#ifdef DEBUG_TRIE
        std::cerr << "Hit recursion_limit: " << recursion_limit_ << "\n";
#else
        throw std::runtime_error( "Grammar-walk-Recursion-Limit-Hit" );
#endif
        return;
    }

    auto metas = sptr.mptr();
    if ( metas != NULL ) {
        for ( auto meta = metas->begin(); meta != metas->end(); ++meta ) {
            std::vector<SectionPtr> current( next );
            for ( auto rule = meta->rbegin(); rule != meta->rend(); ++rule )
                current.push_back( *rule );
            if ( current.size() > 0 ) {
                auto nextRule = current.back();
                current.pop_back();
                walk( nextRule, current, pathSoFar, level+1 );
            }
        }
        return;
    }

    auto rules = sptr.rptr();
    if ( rules != NULL ) {
        for ( auto rule = rules->begin(); rule != rules->end(); ++rule ) {
            pathSoFar.push_back( &( *rule ) );
            if ( next.size() > 0 ) {
                auto ptr = next.back();
                next.pop_back();
                walk( ptr, next, pathSoFar, level+1 );
                next.push_back( ptr );
            }
            else {
                addPathToTrie( pathSoFar );
            }
            pathSoFar.pop_back();
        }
        return;
    }

    throw std::runtime_error( "Grammar-walk-invalid-SectionPtr" );
}



void Grammar::addPathToTrie( std::vector<Rule*> path ) {
#ifdef DEBUG_COUNTS
    Utils::count( "addPathToTrie" );
#endif
    // create an empty rule
    Rule *r = new Rule();

    // concatentate the path rules into r
    for ( auto it = path.begin(); it != path.end(); ++it )
        *r += *(*it);
#ifdef DEBUG_TRIE
    std::cout << "addPathToTrie: " << *r << "\n";
#endif

    // see if this rule has already been added to the trie
    auto data = trie_.match( r->in() );
    if ( data == NULL )
        // not in the trie so add it now
        trie_.addPattern( r->in(), r );
    else if ( data->score() < r->score() ) {
        // its in the trie but this rule has a higher score
        // delete the old path, and save this path
        delete data;
        trie_.addPattern( r->in(), r );
    }
    else
        // we didn't save r above so delete it now
        delete r;
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



