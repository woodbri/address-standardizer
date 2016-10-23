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


SearchPaths Search::search( const std::string &grammarNode, const std::vector<Token> &phrase ) {
    recursion_limit_ = phrase.size() + 2;
    //recursion_limit_ = 40;

    // tracing
#ifdef TRACING_SEARCH
    std::cout << "Calling Search(): recursion_limit: " << recursion_limit_
        << "\nPhrase:";
    for (const auto &t : phrase)
        std::cout << t << "\n";
    std::cout << "\n";
#endif

    // make a list of enumerated token patterns
    std::vector< std::vector<InClass::Type> > list
        = Token::enumerate( phrase );

    // seed the search for each pattern with an empty result
    SearchPaths patterns;
    for (const auto &e : list ) {
        SearchPath pattern;
        pattern.remaining = e;
        patterns.push_back( pattern );
    }

    // do the search and return the results
    // we need to check if we consumed all the tokens
    // and toss out partial matches
    SearchPaths results;
    auto grammarNodePtr = stringToSectionPtr( grammarNode );
    SearchPaths matchs = match( grammarNodePtr, patterns, 0 );
    for ( const auto match : matchs )
        if ( match.remaining.size() == 0 )
            results.push_back( match );


    return results;
}


SearchPaths Search::search( const std::vector<Token> &phrase ) {
    return search( std::string("ADDRESS"), phrase );
}


bool Search::reclassTokens( std::vector<Token> &tokens, const SearchPath &result ) const {
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
                it->inclass( std::set<InClass::Type>() ); // clear it
                it->inclass( in[i] ); // set it to only the type we matched
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


std::vector<Token> Search::searchAndReclassBest( const std::vector<Token> &phrase, float &score ) {
    
    SearchPaths results = search( phrase );
    // if we failed to match against the grammar
    // set score to -1.0 and return an empty result
    if ( results.size() == 0 ) {
        score = -1.0;
        return std::vector<Token>();
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
    std::vector<Token> reclassed( phrase );

    if ( not reclassTokens( reclassed, results[best] ) )
        score = -2.0;

    return reclassed;
}


std::vector<Token> Search::searchAndReclassBest( const std::vector<std::vector<Token> > &phrases, float &score ) {
    
    std::vector<Token> best;
    float bestScore = -1.;

    for ( auto &phrase : phrases ) {
        float thisScore = -1.;
        auto result = searchAndReclassBest( phrase, thisScore );
        if ( thisScore > bestScore ) {
            bestScore = thisScore;
            best = result;
        }
    }

    score = bestScore;
    return best;
}


// ---------------------- PRIVATE ----------------------------


SectionPtr Search::stringToSectionPtr( std::string str ) {
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
    throw std::runtime_error( std::string("Search-Rule-Not-Found:")+str );
}


SearchPaths Search::match( const SectionPtr &sectionPtr, const SearchPaths &paths, const unsigned long int level ) const {
#ifdef TRACING_SEARCH
    std::cout << level << ": Search::match1('" << sectionPtr.name() << "'[";
    for (const auto &t_p : paths ) {
        std::cout << "{";
        for (const auto &t_e : t_p.remaining)
            std::cout << InClass::asString(t_e) << " ";
        std::cout << "}";
    }
    std::cout << "])\n";
#endif

    SearchPaths results;
    for ( const auto &path : paths ) {
        SearchPaths pathResults = match( sectionPtr, path, level );
        for ( const auto result : pathResults )
            results.push_back( result );
    }

#ifdef TRACING_SEARCH
    std::cout << level <<": Returning: Search::match1(" << results.size() << ")\n";
#endif
    return results;
}


SearchPaths Search::match( const SectionPtr &sectionPtr, const SearchPath &path, const unsigned long int level ) const {
#ifdef TRACING_SEARCH
    std::cout << level << ": Search::match2('" << sectionPtr.name()
        << "'(" << sectionPtr.mptr() << ","
        << sectionPtr.rptr() << ")" << "[";
    for (const auto &t_e : path.remaining)
        std::cout << InClass::asString(t_e) << " ";
    std::cout << "])\n";
#endif

    // check for recursion limit
    if ( level > recursion_limit_ ) {
#ifdef TRACING_SEARCH
        std::cout << level << ": Search::match2(0) hit recurssion limit! ###\n";
#endif
        return SearchPaths();
    }

    auto meta = sectionPtr.mptr();
    if ( meta != NULL ) {
#ifdef TRACING_SEARCH
        Utils::count("findMetas");
#endif
        SearchPaths results;
        for ( auto r = meta->begin(); r != meta->end(); ++r ) {
            SearchPath current( path );
            current.next = r->refs();
            for ( const auto &e : path.next )
                current.next.push_back( e );

            if ( current.next.size() > 0 ) {
                auto mresults = matchNext( current, level );
                for ( const auto &mr : mresults )
                    results.push_back( mr );
            }
        }
#ifdef TRACING_SEARCH
        std::cout << level << ": Returning: Search::match2(" << results.size() << ")\n";
#endif
        return results;
    }

    auto rule = sectionPtr.rptr();
    if ( rule != NULL ) {
#ifdef TRACING_SEARCH
        Utils::count("findRules");
#endif
        SearchPaths results;
        for ( auto r = rule->begin(); r != rule->end(); ++r ) {
            auto mr = matchRule( *r, path );
            if ( mr.rules.size() > 0 )
                results.push_back( mr );
        }
#ifdef TRACING_SEARCH
        std::cout << level << ": Returning: Search::match2(" << results.size() << ")\n";
#endif
        return results;
    }

#ifdef TRACING_SEARCH
    std::cout << level << ": Returning: Search::match2(0) [RULE NOT FOUND]\n";
#endif
    return SearchPaths();
}


SearchPaths Search::matchNext( const SearchPaths &paths, const unsigned long int level ) const {
#ifdef TRACING_SEARCH
    std::cout << level << ": Search::matchNext1(paths)\n";
#endif
    SearchPaths results;
    for ( const auto &path : paths ) {
        if ( path.next.size() > 0 ) {
            SearchPaths matches = matchNext( path, level );
            for ( const auto &match : matches )
                results.push_back( match );
        }
    }

#ifdef TRACING_SEARCH
    std::cout << level << ": Returning: Search::matchNext1(" << results.size() << ")\n";
#endif
    return results;
}


SearchPaths Search::matchNext( const SearchPath &path, const unsigned long int level ) const {
#ifdef TRACING_SEARCH
    std::cout << level << ": Search::matchNext2(path)\n";
    std::cout << "\tNext:";
    for (const auto &t_e : path.next )
        std::cout << " " << t_e;
    std::cout << "\n\tRemaining:";
    for (const auto &t_e : path.remaining )
        std::cout << " " << InClass::asString( t_e );
    std::cout << "\n";
#endif

    // we should never call matchNext if there are no next items!
    if ( path.next.size() == 0 ) {
#ifdef TRACING_SEARCH
        std::cout << level << ": Returning: Search::matchNext2(0) NO INPUT\n";
#endif
        return SearchPaths();
    }

    SearchPaths results;
    SearchPath current( path );
    auto sectionPtr = current.next.front();
    current.next.erase( current.next.begin() );

    SearchPaths mr = match( sectionPtr, current, level+1 );
    if ( mr.size() == 0 ) {
#ifdef TRACING_SEARCH
        std::cout << level << ": Returning: Search::matchNext2(0) for '" << sectionPtr.name() << "'\n";
#endif
        return SearchPaths();
    }

    for ( const auto &result : mr ) {
        if ( result.next.size() > 0 ) {
            SearchPaths nextResults = matchNext( mr, level );
            for ( const auto &result : nextResults )
                results.push_back( result );
        }
        else {
            results.push_back( result );
        }
    }

#ifdef TRACING_SEARCH
    std::cout << level << ": Returning: Search::matchNext2(" << results.size() << ")\n";
#endif
    return results;
}


SearchPath Search::matchRule( const Rule &r, const SearchPath &path ) const {
#ifdef TRACING_SEARCH
    std::cout << "Search::matchRule\n";
#endif
    auto &pattern = path.remaining;

    if ( r.inSize() > pattern.size() ) {
        // failed to match, rule has more items than pattern
        // return an empty result
#ifdef TRACING_SEARCH
        std::cout << "\tReturning: Search::matchRule: Failed 1\n";
#endif
        return SearchPath();
    }

    // try to match the rule to the pattern
    auto it = pattern.begin();
    for ( long unsigned int pos = 0; pos < r.inSize(); ++pos ) {
        if ( *it == r.in( pos ) )
            it++;
        else {
            // failed to match
#ifdef TRACING_SEARCH
            std::cout << "\tReturning: Search::matchRule: Failed 2\n";
#endif
            return SearchPath();
        }
    }

    // rule was matched, so return it
    // and take what remains unmatched of pattern
    // and copy it to remaining
    SearchPath result( path );
    result.rules.push_back(r);
    result.remaining.clear();
    for (;it!=pattern.end(); it++)
        result.remaining.push_back(*it);

#ifdef TRACING_SEARCH
    std::cout << "\tReturning: Search::matchRule: rule[" << r << "], remaining:[";
    for (const auto &t_e : result.remaining)
        std::cout << InClass::asString(t_e) << " ";
    std::cout << "]\n";
#endif
    return result;
}



