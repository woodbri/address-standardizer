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

#include "search.h"


bool Search::search() {
    // tracing
    std::cout << "Calling Search()\nPattern:";
    for (const auto &e : pattern_)
        std::cout << " " << InClass::asString( e );
    std::cout << "\n";

    if ( pattern_.size() == 0 ) return false;
    results_.clear();
    unsigned int pos = 0;
    unsigned int stackDepth = 0;
    stack_.clear();

    if ( match( std::string("ADDRESS"), 0, pos, stackDepth ) ) {
        results_.push_back( stack_ );
        return true;
    }
    
    return false;
}


bool Search::search( const std::vector<InClass::Type> &pattern ) {
    pattern_ = pattern;
    return search();
}


std::vector<Rule> Search::bestResult() const {
    // if not results return an empty vector
    if (results_.size() == 0) {
        std::vector<Rule> dummy;
        return dummy;
    }

    // for each result compute the average score of the rules in the result
    // and select the record with the best average score
    int best = 0;
    float bestScore = 0.0;
    int i = 0;
    for ( const auto &result : results_ ) {
        float sum = 0.0;
        for ( const auto &rule : result )
            sum += rule.score();
        sum /= result.size();
        if (sum > bestScore) {
            best = i;
            bestScore = sum;
        }
        ++i;
    }

    return results_[best];
}


bool Search::matchAllMeta(const Rule &rule, const int level, unsigned int pos, unsigned int stackDepth) {
    std::cout << std::string(level, '.') << "Calling matchAllMeta(\"" << rule <<"\")(" << pos << ", " << stackDepth << ")[" << pos_ << ", " << stack_.size() << "]\n";
    std::vector<std::string> meta = rule.meta();
    for (const auto &m : meta )
        if ( not match( m, level+1, pos, stackDepth ) ) {
            std::cout << std::string(level, '.') << "failed on '"
                << m << "'\n";
            // unwind stack and pos pointer
            if (stack_.size() > stackDepth)
                stack_.resize(stackDepth);
            pos_ = pos;
            return false;
        }
        else
            pos = pos_;

    std::cout << std::string(level, '.') << "return true\n";
    return true;
}

bool Search::match(const std::string &name, const int level, unsigned int pos, unsigned int stackDepth) {
    std::cout << std::string(level, '.') << "Calling match(\"" << name << "\")(" << pos << ", " << stackDepth << ")[" << pos_ << ", " << stack_.size() << "]\n";
    auto it = rules_.find( name );
    if ( it == rules_.end() ) {
        std::cout << std::string(level, '.') << 
                "Failed to find rule for '" << name << "'\n";
        // unwind stack and pos pointer
        if (stack_.size() > stackDepth)
            stack_.resize(stackDepth);
        pos_ = pos;
        return false;
    }

    for ( const auto &r : (*it).second ) {
        if ( r.isMeta() ) {
            if ( matchAllMeta( r, level+1, pos, stackDepth ) ) {
                std::cout << std::string(level, '.') << "return true\n";
                return true;
            }
        }
        else {
            if ( match( r, level+1, pos, stackDepth ) ) {
                std::cout << std::string(level, '.') << "return true\n";
                return true;
            }
        }
    }

    std::cout << std::string(level, '.') << "return false\n";
    if (stack_.size() > stackDepth)
        stack_.resize(stackDepth);
    pos_ = pos;
    return false;
}


bool Search::match(const Rule &rule, const int level, unsigned int pos, unsigned int stackDepth) {
    std::cout << std::string(level, '.') << "Calling match(\"" << rule << "\")(" << pos << ", " << stackDepth << ")[" << pos_ << ", " << stack_.size() << "]\n";
    std::vector<InClass::Type> in = rule.in();
    for (const auto &e : in) {
        if (pos < pattern_.size() and pattern_[pos] == e)
            ++pos;
        else {
            std::cout << std::string(level, '.') << "return false\n";
            return false;
        }
    }

    // advance the pointer into pattern on success
    // and save rule on stack
    pos_ = pos;
    stack_.push_back( rule );

    std::cout << std::string(level, '.') << "return true\n";
    return true;
}



