/*HEADER*
*HEADER*/

#include <iostream>

#include "search.h"


bool Search::search() {
    // tracing
    std::cout << "Calling Search()\nPattern:";
    for (const auto &e : pattern_)
        std::cout << " " << InClass::asString( e );
    std::cout << "\n";

    if ( pattern_.size() == 0 ) return false;
    pos_ = 0;
    results_.clear();
    stack_.clear();

    if ( match( std::string("ADDRESS"), 0 ) ) {
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


bool Search::matchAllMeta(const Rule &rule, const int lvl) {
    std::vector<std::string> meta = rule.meta();
    for (const auto &m : meta )
        if ( not match( m, lvl+1 ) ) {
            std::cout << std::string(lvl, '.') << "failed on '"
                << m << "'\n";
            return false;
        }

    std::cout << std::string(lvl, '.') << "return true\n";
    return true;
}

bool Search::match(const std::string &name, const int lvl) {
    std::cout << std::string(lvl, '.') << "Calling match(\"" << name << "\")\n";
    auto it = rules_.find( name );
    if ( it == rules_.end() ) {
        std::cout << std::string(lvl, '.') << 
                "Failed to find rule for '" << name << "'\n";
        return false;
    }

    for ( const auto &r : (*it).second ) {
        if ( r.isMeta() ) {
            if ( matchAllMeta( r, lvl ) ) {
                std::cout << std::string(lvl, '.') << "return true\n";
                return true;
            }
        }
        else {
            if ( match( r, lvl+1 ) ) {
                std::cout << std::string(lvl, '.') << "return true\n";
                return true;
            }
        }
    }

    std::cout << std::string(lvl, '.') << "return false\n";
    return false;
}


bool Search::match(const Rule &rule, const int lvl) {
    std::cout << std::string(lvl, '.') << "Calling match(\"" << rule << "\")\n";
    int pos = pos_;
    std::vector<InClass::Type> in = rule.in();
    for (const auto &e : in) {
        if (pos < (int) pattern_.size() and pattern_[pos] == e)
            ++pos;
        else {
            std::cout << std::string(lvl, '.') << "return false\n";
            return false;
        }
    }

    // advance the pointer into pattern on success
    // and save rule on stack
    pos_ = pos;
    stack_.push_back( rule );

    std::cout << std::string(lvl, '.') << "return true\n";
    return true;
}



