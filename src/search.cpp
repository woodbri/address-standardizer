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
#ifdef TRACING_SEARCH
    std::cout << "Calling Search()\nPattern:";
    for (const auto &e : pattern_)
        std::cout << " " << InClass::asString( e );
    std::cout << "\n";
#endif

    if ( pattern_.size() == 0 ) return false;
    results_.clear();
    unsigned int pos = 0;
    stack_.clear();

    if ( match( std::string("ADDRESS"), 0, pos ) ) {
        results_.push_back( stack_ );
        return true;
    }
    
    return false;
}


bool Search::search( const std::vector<InClass::Type> &pattern ) {
    pattern_ = pattern;
    return search();
}


std::vector<Rule> Search::bestResult() {
    float score;
    return bestResult( score );
}


std::vector<Rule> Search::bestResult( float &score ) {
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

    score = bestScore;

    return results_[best];
}


std::vector<Rule> Search::bestResult( const std::vector< std::vector<InClass::Type> > &list ) {
    std::vector<Rule> bestResult;
    float bestScore;
    for (const auto &e : list) {
        if ( search( e ) ) {
            float score;
            auto best = bestResult( score );
            if ( score > bestScore ) {
                bestResult = best;
                bestScore = score;
            }
        }
    }

    return bestResult;
}


bool Search::matchAllMeta(const Rule &rule, const int level, unsigned int pos) {
    const unsigned int stackDepth(stack_.size());

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "Calling matchAllMeta(\""
        << rule <<"\")(" << pos << ", " << stackDepth << ")"
        "[" << pos_ << ", " << stack_.size() << "]\n";
#endif

    std::vector<std::string> meta = rule.meta();
    for (const auto &m : meta )
        if ( not match( m, level+1, pos ) ) {
#ifdef TRACING_SEARCH
            std::cout << std::string(level, '.') << "failed on '"
                << m << "'\n";
#endif
            // unwind stack and pos pointer
            if (stack_.size() > stackDepth)
                stack_.resize(stackDepth);
            pos_ = pos;
            return false;
        }
        else
            pos = pos_;

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "return true\n";
#endif
    return true;
}



bool Search::match(const std::string &name, const int level, unsigned int pos) {
    const unsigned int stackDepth(stack_.size());

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "Calling match(\""
        << name << "\")(" << pos << ", " << stackDepth << ")"
        "[" << pos_ << ", " << stack_.size() << "]\n";
#endif

    auto it = rules_.find( name );
    if ( it == rules_.end() ) {
#ifdef TRACING_SEARCH
        std::cout << std::string(level, '.') << 
                "Failed to find rule for '" << name << "'\n";
#endif
        // unwind stack and pos pointer
        if (stack_.size() > stackDepth)
            stack_.resize(stackDepth);
        pos_ = pos;
        return false;
    }

    for ( const auto &r : (*it).second ) {
        if ( r.isMeta() ) {
            std::vector<std::string> meta = r.meta();
            auto start = meta.begin();
            auto remaining = start;
            ++remaining;
            auto end = meta.end();
            if ( match( start, remaining, end, level+1, pos ) ) {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "return true\n";
#endif
                return true;
            }
            else {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "continuing\n";
#endif
                if (stack_.size() > stackDepth)
                    stack_.resize(stackDepth);
                pos_ = pos;
                continue;
            }
        }
        else {
            if ( match( r, level+1, pos ) ) {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "return true\n";
#endif
                return true;
            }
            else {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "continuing\n";
#endif
                if (stack_.size() > stackDepth)
                    stack_.resize(stackDepth);
                pos_ = pos;
                continue;
            }
        }
    }

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "return false\n";
#endif
    if (stack_.size() > stackDepth)
        stack_.resize(stackDepth);
    pos_ = pos;
    return false;
}


bool Search::match2(const std::string &name, const int level, unsigned int pos) {
    const unsigned int stackDepth(stack_.size());

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "Calling match(\""
        << name << "\")(" << pos << ", " << stackDepth << ")"
        "[" << pos_ << ", " << stack_.size() << "]\n";
#endif

    auto it = rules_.find( name );
    if ( it == rules_.end() ) {
#ifdef TRACING_SEARCH
        std::cout << std::string(level, '.') << 
                "Failed to find rule for '" << name << "'\n";
#endif
        // unwind stack and pos pointer
        if (stack_.size() > stackDepth)
            stack_.resize(stackDepth);
        pos_ = pos;
        return false;
    }

    for ( const auto &r : (*it).second ) {
        if ( r.isMeta() ) {
            if ( matchAllMeta( r, level+1, pos ) ) {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "return true\n";
#endif
                return true;
            }
        }
        else {
            if ( match( r, level+1, pos ) ) {
#ifdef TRACING_SEARCH
                std::cout << std::string(level, '.') << "return true\n";
#endif
                return true;
            }
        }
    }

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "return false\n";
#endif
    if (stack_.size() > stackDepth)
        stack_.resize(stackDepth);
    pos_ = pos;
    return false;
}


bool Search::match(const Rule &rule, const int level, unsigned int pos) {

#ifdef TRACING_SEARCH
    const unsigned int stackDepth(stack_.size());
    std::cout << std::string(level, '.') << "Calling match(\""
        << rule << "\")(" << pos << ", " << stackDepth << ")"
        "[" << pos_ << ", " << stack_.size() << "]\n";
#endif

    std::vector<InClass::Type> in = rule.in();
    for (const auto &e : in) {
        if (pos < pattern_.size() and pattern_[pos] == e)
            ++pos;
        else {
#ifdef TRACING_SEARCH
            std::cout << std::string(level, '.') << "return false\n";
#endif
            return false;
        }
    }

    // advance the pointer into pattern on success
    // and save rule on stack
    pos_ = pos;
    stack_.push_back( rule );

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "return true\n";
#endif

    return level == level; // dummy return true to avoid warning
}


bool Search::match(VecStringIter start, VecStringIter  next, VecStringIter end, const int level, unsigned int pos) {
    const unsigned int stackDepth(stack_.size());

#ifdef TRACING_SEARCH
    std::cout << std::string(level, '.') << "Calling iter match(\""
        << *start << ", next, end, " << level << ", " << pos 
        << ")\n";
#endif
    if ( match( *start, level+1, pos ) ) {
        if ( next == end ) {
#ifdef TRACING_SEARCH
            std::cout << std::string(level, '.') << "pos_: "
                << pos_ << ", len: " << pattern_.size()
                << ((pos_ == (int)pattern_.size())?" true": " false") << "\n";
#endif
            return true;
        }
        start = next;
        ++next;
        if ( match( start, next, end, level+1, pos_ ) ) {
#ifdef TRACING_SEARCH
            std::cout << std::string(level, '.') << "return true\n";
#endif
            return true;
        }
        else {
#ifdef TRACING_SEARCH
            std::cout << std::string(level, '.') << "return false\n";
#endif
            if (stack_.size() > stackDepth)
                stack_.resize(stackDepth);
            pos_ = pos;
            return false;
        }
    }
    else {
#ifdef TRACING_SEARCH
        std::cout << std::string(level, '.') << "return true\n";
#endif
        if (stack_.size() > stackDepth)
            stack_.resize(stackDepth);
        pos_ = pos;
        return false;
    }
}
