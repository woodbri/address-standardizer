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


MatchResults Search::search( const std::string &grammarNode, const std::vector<Token> &phrase ) const {
    // tracing
#ifdef TRACING_SEARCH
    std::cout << "Calling Search()\nPhrase:";
    for (const auto &t : phrase)
        std::cout << t << "\n";
    std::cout << "\n";
#endif

    // make a list of enumerated token patterns
    std::vector< std::vector<InClass::Type> > list
        = Token::enumerate( phrase );

    // seed the search for each pattern with an empty result
    MatchResults patterns;
    for (const auto &e : list ) {
        MatchResult pattern;
        pattern.remaining = e;
        patterns.push_back( pattern );
    }

    // do the search and return the results
    return match( grammarNode, patterns );
}


MatchResults Search::search( const std::vector<Token> &phrase ) const {
    return search( std::string("ADDRESS"), phrase );
}


bool Search::reclassTokens( std::vector<Token> &tokens, const MatchResult &result ) const {
    auto rules = result.rules;

    // count the tokens in the rules and compare to tokens
    long unsigned int cnt = 0;
    for ( const auto &r : rules )
        cnt += r.inSize();
    if ( cnt != tokens.size() )
        return false;

    // set the outClass attribute on the tokens based on the search results
    std::vector<Token>::iterator it = tokens.begin();
    for ( const auto &r : rules ) {
        const auto in = r.in();
        const auto out = r.out();
        for ( long unsigned int i=0; i<in.size(); ++i ) {
            if ( it->isInClass( in[i] ) ) {
                it->outclass( out[i] );
                ++it;
            }
            else {
                std::cerr << "ERROR: Search::reclassTokens: Results do not match tokens!\n";
                return false;
            }
        }
    }

    return true;
}


MatchResult Search::searchAndReclassBest( std::vector<Token> &phrase, float &score ) const {
    
    MatchResults results = search( phrase );
    // if we failed to match against the grammar
    // set score to -1.0 and return an empty result
    if ( results.size() == 0 ) {
        score = -1.0;
        return MatchResult();
    }

    // for each result compute the average score of the rules in the result
    // and select the record with the best average score
    int best = 0;
    float bestScore = 0.0;
    int i = 0;
    for ( const auto &result : results ) {
        float sum = 0.0;
        for ( const auto &rule : result.rules )
            sum += rule.score();
        sum /= static_cast<float>( result.rules.size() );
        if (sum > bestScore) {
            best = i;
            bestScore = sum;
        }
        ++i;
    }

    score = bestScore;

    if ( not reclassTokens( phrase, results[best] ) )
        score = -2.0;

    return results[best];
}


/*
std::vector<Rule> Search::bestResult( const std::vector< std::vector<InClass::Type> > &list ) const {
    std::vector<Rule> bestResult;
    float bestScore;
    for (const auto &e : list) {
        if ( search( e ) ) {
            float score;
            auto best = this->bestResult( score );
            if ( score > bestScore ) {
                bestResult = best;
                bestScore = score;
            }
#if defined(TRACING_SEARCH) || defined(TRACING_SEARCH_MIN)
            std::cout << "PATTERN:";
            for (const auto &t : e)
                std::cout << " " << InClass::asString(t);
            std::cout << "\t(" << score << ")\n";
        }
        else {
            std::cout << "PATTERN:";
            for (const auto &t : e)
                std::cout << " " << InClass::asString(t);
            std::cout << "\t(failed)\n";
#endif
        }
    }

    return bestResult;
}
*/

// ---------------------- PRIVATE ----------------------------


MatchResults Search::match( const std::string &name, const MatchResults &results ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::match('" << name << "'[";
    for (const auto &t_e : results) {
        std::cout << "{";
        for (const auto &t_f : t_e.remaining)
            std::cout << InClass::asString(t_f) << " ";
        std::cout << "}";
    }
    std::cout << "])\n";
#endif
    MatchResults mresults;
    for ( const auto &r : results ) {
        auto matches = match( name, r );
        for ( const auto mr : matches )
            mresults.push_back( mr );
    }
#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << mresults.size() << ")\n";
#endif
    return mresults;
}


MatchResults Search::match( const std::string &name, const MatchResult &result ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::match('" << name << "'[";
    for (const auto &t_e : result.remaining)
        std::cout << InClass::asString(t_e) << " ";
    std::cout << "])\n";
#endif

    auto meta = metas_.find( name );
    if ( meta != metas_.end() ) {
        MatchResults results;
        for ( const auto &r : (meta->second).rules() ) {
            auto mresults = matchMeta( r, result );
            if ( mresults.size() > 0 )
                for ( const auto &mr : mresults )
                    results.push_back( mr );
        }
#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << results.size() << ")\n";
#endif
        return results;
    }

    auto rule = rules_.find( name );
    if ( rule != rules_.end() ) {
        MatchResults results;
        for ( const auto &r : (rule->second).rules() ) {
            auto mr = matchRule( r, result.remaining );
            if ( mr.rules.size() > 0 )
                results.push_back( mr );
        }
#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << results.size() << ")\n";
#endif
        return results;
    }

#ifdef TRACING_SEARCH
    std::cout << "\tReturning(0) [RULE NOT FOUND]\n";
#endif
    return MatchResults();
}


MatchResults Search::matchMeta( const MetaRule &r, const MatchResults &results ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::matchMeta([" << r << "],[";
    for (const auto &t_e : results) {
        std::cout << "{";
        for (const auto &t_f : t_e.remaining)
            std::cout << InClass::asString(t_f) << " ";
        std::cout << "}";
    }
    std::cout << "])\n";
#endif
    MatchResults mresults;
    for ( const auto &result : results ) {
        auto matches = matchMeta( r, result );
        for ( const auto mr : matches )
            mresults.push_back( mr );
    }

#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << mresults.size() << ")\n";
#endif
    return mresults;
}


MatchResults Search::matchMeta( const MetaRule &r, const MatchResult &result ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::matchMeta([" << r << "],[";
    for (const auto &t_e : result.remaining)
        std::cout << InClass::asString(t_e) << " ";
    std::cout << "])\n";
#endif
    MatchResults mresults;
    for ( const auto &name : r.refs() ) {
        auto matches = match( name, result );
        if ( matches.size() == 0 ) {
            // we failed to match this rule
            // so return an empty result
#ifdef TRACING_SEARCH
    std::cout << "\tReturning(0) [FALED TO MATCH]\n";
#endif
            return MatchResults();
        }

        auto merged = mergeResults( result, matches );
        for ( const auto &m : merged )
            mresults.push_back( m );
    }

#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << mresults.size() << ")\n";
#endif
    return mresults;
}


MatchResults Search::mergeResults( const MatchResult &orig, const MatchResult &added ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::mergeResults(orig,added)\n";
#endif
    MatchResults mresults;

    MatchResult result( orig );
    for ( const auto &r : added.rules )
        result.rules.push_back( r );
    result.remaining = added.remaining;
    mresults.push_back( result );

#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << mresults.size() << ")\n";
#endif
    return mresults;
}


MatchResults Search::mergeResults( const MatchResult &orig, const MatchResults &added ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::mergeResults(orig,adds)\n";
#endif
    MatchResults mresults;

    for ( const auto &r : added ) {
        auto merged = mergeResults( orig, r );
        for ( const auto &m : merged )
            mresults.push_back( m );
    }

#ifdef TRACING_SEARCH
    std::cout << "\tReturning(" << mresults.size() << ")\n";
#endif
    return mresults;
}


MatchResult Search::matchRule( const Rule &r, const std::vector<InClass::Type> pattern ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::matchRule\n";
#endif
    MatchResult result;
    auto it = pattern.begin();
    for (const auto &e : r.in()) {
        if ( it == pattern.end() ) {
            // failed to match, rules has more items than pattern
            // return an empty result
#ifdef TRACING_SEARCH
            std::cout << "\tReturning Failed 1\n";
#endif
            return result;
        }

        if ( *it == e )
            it++;
        else {
            // failed to match
#ifdef TRACING_SEARCH
            std::cout << "\tReturning Failed 2\n";
#endif
            return result;
        }
    }

    // we have a match so update result and return it
    result.rules.push_back(r);
    for (;it!=pattern.end(); it++)
        result.remaining.push_back(*it);

#ifdef TRACING_SEARCH
    std::cout << "\tReturning Matched: rule[" << r << "], remaining:[";
    for (const auto &t_e : result.remaining)
        std::cout << InClass::asString(t_e) << " ";
    std::cout << "]\n";
#endif
    return result;
}



