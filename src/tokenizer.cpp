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

#include <vector>
#include <string>

#include "tokenizer.h"
#include "utils.h"

void Tokenizer::removeFilter(InClass::Type filter) {
    auto it = filter_.find(filter);
    if ( it != filter_.end() )
        filter_.erase(it);
}

std::vector<std::vector<Token> > Tokenizer::getTokens(std::string str) {
    std::vector<std::vector<Token> > phrases;

    std::vector<Token> phrase1 = applyFilter( getTokens( str, false ) );
    phrases.push_back( phrase1 );

    std::vector<Token> phrase2 = applyFilter( recombine( getTokens( str, true ) ) );
    if ( phrase2.size() > 0 )
        phrases.push_back( phrase2 );

    return phrases;
}


std::vector<Token> Tokenizer::getTokens( std::string str, bool splitTokens ) {

    // make sure the text is normalized and UUPERCASE
    std::string locale = lex_.locale();
    UErrorCode errorCode;
    std::string nstr = Utils::normalizeUTF8( str, errorCode );
    str = Utils::upperCaseUTF8( nstr, locale );

    // first see if we have attached tokens that we need to split apart
    // we place and em dash utf8 char "\xe2\x80\x94" where we split the word
    // this can later be recognized as EMDASH token if needed
    bool didSplit = false;
    std::string attached = "(" + lex_.regexPrefixAtt() + ")";
    if (attached.length() > 2) {
        boost::u32regex re = boost::make_u32regex( attached );
        const char* replace( "$1\xe2\x80\x94" );
        std::string tmp = boost::u32regex_replace( str, re, replace );
        str = tmp;
        didSplit = true;
    }

    attached = "(" + lex_.regexSuffixAtt() + ")";
    if (attached.length() > 2) {
        boost::u32regex re = boost::make_u32regex( attached );
        const char* replace( "\xe2\x80\x94$1" );
        std::string tmp = boost::u32regex_replace( str, re, replace );
        str = tmp;
        didSplit = true;
    }

    // if we are asked for to split tokens
    // but there were no regex for splitting
    // then we return an empty result.
    // the rationale is that this would be a duplicate
    // set of tokens to those returned by not splitTokens
    if ( splitTokens and not didSplit )
        return std::vector<Token>();

    // split mixed alpha digit tokens, eg:
    // 500W => 500 W or N123 => N 123 or I80 => I 80
    const char* replace( "$1 $2" );
    attached = "\\<(\\d+)([[:alpha:]\\p{L}])\\>";
    boost::u32regex re = boost::make_u32regex( attached );
    std::string tmp = boost::u32regex_replace( str, re, replace );
    str = tmp;
    attached = "\\<([[:alpha:]\\p{L}])(\\d+)\\>";
    re = boost::make_u32regex( attached );
    tmp = boost::u32regex_replace( str, re, replace );
    str = tmp;

    // build the regex for identifying tokens
    std::string regex = "^\\s*(?!(?:\xe2\x80\x94)+)(" + lex_.regex() + "\\d+/\\d+|\\d+|\\<[[:alpha:]]+\\>|[\\p{L}\\p{Nd}]+|[[:alpha:]\\d]+)([-&\\s\\|[:punct:]]+|\xe2\x80\x94|$)";

    re = boost::make_u32regex( regex, boost::regex::icase );

    std::vector<Token> outtokens;

    boost::smatch what;
    boost::match_flag_type flags = boost::match_default;

    // could not get this to work with auto
    std::string::const_iterator start, end;
    start = str.begin();
    end   = str.end();

    while ( boost::u32regex_search( start, end, what, re ) ) {
        // fetch and create the token for the word or phrase
        if (what[1].first < what[1].second) {
            Token tok( std::string(what[1].first, what[1].second) );
            // token might classify as multiple types
            // or none, in which case make it a word
            lex_.classify(tok, InClass::WORD);
            outtokens.push_back(tok);
        }

        // create a token for the punctuation
        if (what[2].first < what[2].second) {
            Token punct( std::string(what[2].first, what[2].second) );
            lex_.classify(punct, InClass::PUNCT);
            outtokens.push_back(punct);
        }

        // update search position
        start = what[0].second;

        // update flags
        flags |= boost::match_not_bob;
    }

    return outtokens;
}


std::vector<Token> Tokenizer::recombine( const std::vector<Token> &in ) {
    
    std::vector<Token> out;

    // if there is nothing to do then return
    if ( in.size() == 0 )
        return out;

    // push the first token to the output stream
    out.push_back( in[0] );
    unsigned long int i=1;
    for ( ; i<in.size()-1; ++i ) {
        // if this token is an EMDASH see if we need to rejoin
        // the tokens to the right and left of it
        if ( in[i].isInClass( InClass::EMDASH ) ) {
            std::string text = in[i-1].text() + in[i+1].text();

            // see if the recombined token is in the lexicon
            auto le = lex_.find( text );
            if ( le.size() == 0 ) {
                // not in the lexicon so keep them as is
                out.push_back( in[i] );
                continue;
            }
            else {
                // yes recombined token is in the lexicon
                // so undo the split and toss the emdash
                out[i-1].text( text );
                lex_.classify( out[i-1], InClass::WORD );
                // skip over the emdash AND the next token
                i += 2;
                continue;
            }
        }
        out.push_back( in[i] );
    }
    // and push the last if we need to
    if ( i < in.size() )
        out.push_back( in[in.size()-1] );

    return out;
}


std::vector<Token> Tokenizer::applyFilter( const std::vector<Token> &in ) {
    std::vector<Token> out;

    for (const auto &t : in )
        if (not t.isInClass(filter_))
            out.push_back( t );

    return out;
}
