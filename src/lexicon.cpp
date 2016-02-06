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
#include <string>
#include <algorithm>

#include "utils.h"
#include "lexentry.h"
#include "inclass.h"
#include "lexicon.h"

// constructors

Lexicon::Lexicon() {
    name_ = "";
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();
}


Lexicon::Lexicon(std::string name) {
    name_ = name;
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();
}


Lexicon::Lexicon(std::string name, std::string file) {
    name_ = name;
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();

    std::string line;
    std::ifstream is (file.c_str(), std::ifstream::in|std::ifstream::binary);
    if (not is.is_open()) {
        std::cerr << "ERROR: Failed to open file: '" << file << "'\n";
        //throw 1000;
        return;
    }

    // read the header
    std::getline( is, line );

    // echo out the read line indented for debugging
    //std::cout << "\t" << line << "\n";

    // replace multiple space chars with a single space
    // but don't touch tab chars
    boost::regex re("( +)");
    const char* replace = " ";
    std::string tmp = boost::regex_replace(line, re, replace);
    line = tmp;

    std::stringstream buffer(line);
    std::string term;
    std::getline(buffer, term, '\t');
    if (term != "LEXICON:" and term != "Lexicon:") {
        is.close();
        std::cout << "ERROR: file format error: '" << file << "'\n";
        //throw 1001;
        return;
    }

    std::getline(buffer, term, '\t');   // name (ignored)
    std::getline(buffer, term, '\t');   // lang

    lang_ = InClass::asLang( term );        // set the language
    std::getline(buffer, locale_, '\t');    // set the locale
                                            // ignore the count of keys

    UErrorCode errorCode;

    // ret in the lexicon entries
    while (not is.eof()) {
        std::getline( is, line );
            //std::cout << "\t" << line << "\n";
        if (line.length() == 0) continue;

        if ( line.compare(0, 9, "LEXENTRY:") == 0 or
             line.compare(0, 9, "LexEntry:") == 0 ) {
            if (locale_ != "") {
                std::string nstr = Utils::normalizeUTF8( line, errorCode );
                line = Utils::upperCaseUTF8( nstr, locale_ );
            }
            LexEntry le( line );
            insert( le );
        }
        else {
            std::cout << "WARNING: skipping: " << line << "\n";
        }
    }
    is.close();
}

// getters


std::vector<LexEntry> Lexicon::find( const std::string key ) {

    std::vector<LexEntry> empty;

    auto it = lex_.find( key );
    if ( it != lex_.end() )
        return (*it).second;
    else
        return empty;
}


// operators

std::ostream &operator<<(std::ostream &ss, const Lexicon &lex) {
    ss << "LEXICON:" << "\t"
       << lex.name_ << "\t"
       << lex.langAsString() << "\t"
       << lex.locale_ << "\t"
       << lex.lex_.size() << "\n";

    // each lexicon entry is a vector of LexEntry objects
    for (const auto &l  : lex.lex_)
        for (const auto &d : l.second)
            ss << d << "\n";

    return ss;
}


void Lexicon::classify( Token& token, InClass::Type typ ) {
    
    // fetch the entry from the lexicon
    // we get an empty container if it is not found
    std::string text = token.text();
    std::vector<LexEntry> entries = find( text );

    // append appropriate classes to token
    for( const auto &entry : entries ) {
        // for each entry add it to the token classification
        std::set<InClass::Type> letype = entry.type();
        for ( const auto &e : letype )
            token.inclass( e );
    }

    if ( not token.isInClassEmpty() ) return;

    boost::regex num_exp("^\\d+$");
    boost::regex word_exp("^\\w+$");
    boost::regex has_digit("^.*\\d.*$");
    boost::regex dash_exp("^[-]+$");
    boost::regex emdash_exp("^\xe2\x80\x94$");
    boost::regex punct_exp("^[-&\\s\\|[:punct:]]+$");
    boost::regex space_exp("^\\s+$");
    boost::regex fract_exp("^\\d+/\\d+$");
    boost::regex pch_exp("^[A-Z]{1,2}\\d{1,2}[A-Z]{0,1}$", boost::regex::icase);
    boost::regex pct_exp("^(\\d[A-Z]\\d|\\d[A-Z]{2})$", boost::regex::icase);

    // is it a number
    if (boost::regex_match(text, num_exp, boost::match_default)) {
        token.inclass( InClass::NUMBER );
        if (text.length() == 4)
            token.inclass( InClass::QUAD );
        if (text.length() == 5)
            token.inclass( InClass::QUINT );
    }
    else if ( text == "@" ) {
        token.inclass( InClass::ATSIGN );
    }
    else if ( text == "/" ) {
        token.inclass( InClass::SLASH );
    }
    // is it a fract
    else if (boost::regex_match(text, fract_exp, boost::match_default)) {
        token.inclass( InClass::FRACT );
    }
    // is it pch
    else if (boost::regex_match(text, pch_exp, boost::match_default)) {
        token.inclass( InClass::MIXED );
        token.inclass( InClass::PCH );
    }
    // is it pct
    else if (boost::regex_match(text, pct_exp, boost::match_default)) {
        token.inclass( InClass::MIXED );
        token.inclass( InClass::PCT );
    }
    // is it alpha
    // is mixed alpha and digit
    else if (boost::regex_match(text, word_exp, boost::match_default)) {
        if (boost::regex_match(text, has_digit, boost::match_default))
            token.inclass( InClass::MIXED );
        else {
            if (text.length() == 2)
                token.inclass( InClass::DOUBLE );
            else if (text.length() == 1)
                token.inclass( InClass::SINGLE );

            token.inclass( InClass::WORD );
        }
    }
    // is it emdash
    else if (boost::regex_match(text, emdash_exp, boost::match_default)) {
        token.inclass( InClass::EMDASH );
    }
    // is it dash
    else if (boost::regex_match(text, dash_exp, boost::match_default)) {
        token.inclass( InClass::DASH );
        token.inclass( InClass::PUNCT );
    }
    // is it ampersand
    else if (text == "&") {
        token.inclass( InClass::AMPERS );
        token.inclass( InClass::PUNCT );
    }
    // is it punct
    else if (boost::regex_match(text, punct_exp, boost::match_default)) {
        token.inclass( InClass::PUNCT );
    }
    // is it whitespace
    else if (boost::regex_match(text, space_exp, boost::match_default)) {
        token.inclass( InClass::SPACE );
    }

    if ( (token.inclass()).empty() )
        token.inclass( typ );
}


// mutators

void Lexicon::insert( const LexEntry &le ) {

    // get the map key we want to find
    std::string key = le.word();

    // fetch the entry from the lexicon
    // we get an empty container if it is not found
    std::vector<LexEntry> entries = find( key );

    // see if it is already here and do nothing if it is
    for( const auto &entry : entries )
        if ( entry == le )
            return;

    // append the lexentry to the existing entries for this key
    entries.push_back( le );

    // save it back in the lexicon
    lex_[key] = entries;

    // set cached regex string to empty
    // so it will get regenerated
    regex_ = "";
}


void Lexicon::remove( const LexEntry &le ) {
    
    // get the map key we want to find
    std::string key = le.word();

    // fetch the entry from the lexicon
    // we get an empty container if it is not found
    std::vector<LexEntry> entries = find( key );
    if (entries.size() == 0)
        return;

    // see if it is already here and do nothing if it is
    for( auto entry=entries.begin(); entry!=entries.end(); entry++ ) {
        if ( *entry == le ) {
            entries.erase( entry );

            if ( entries.size() == 0 ) {
                // remove the key from the lexicon
                lex_.erase( key );
            }
            else {
                // save it back in the lexicon
                lex_[key] = entries;
            }
            break;
        }
    }

    // set cached regex strings to empty
    // so it will get regenerated
    regex_.clear();
    regexPrefix_.clear();
    regexSuffix_.clear();
}


bool sortByStringLength(std::string a, std::string b) {
    return a.length() > b.length();
}


std::string Lexicon::regex() {

    // if the regex string is empty, regenerate it
    if (regex_.length() == 0) {
        // collect all lexicon keys in vector
        std::vector<std::string> keys;
        for ( const auto &e : lex_ )
            keys.push_back( e.first );

        // sort them based on length desc
        std::sort(keys.begin(), keys.end(), sortByStringLength);

        // concat them into a regex fragment
        std::string str;
        for ( const auto &key : keys ) {
            std::string outkey = escapeRegex( key );
            str += outkey + "|";
        }

        regex_ = str;
    }
    
    return regex_;
}


std::string Lexicon::regexPrefixAtt() {

    if (regexPrefix_.length() == 0) {
        std::vector<std::string> prefix;

        for ( const auto &e : lex_ )
            for ( const auto &le : e.second )
                if ( le.isPrefixAttached() )
                        prefix.push_back(e.first);

        // sort them based on length desc
        std::sort(prefix.begin(), prefix.end(), sortByStringLength);

        // concat them into a regex fragment
        std::string str;
        for ( const auto &e : prefix ) {
            std::string ee = escapeRegex( e );
            str += "\\b" + ee + "\\B|";
        }

        // remove trailing '|' char
        if (str.length() > 0)
            str.pop_back();

        regexPrefix_ = str;
    }

    return regexPrefix_;
}


std::string Lexicon::regexSuffixAtt() {

    if (regexSuffix_.length() == 0) {
        std::vector<std::string> suffix;

        for ( const auto &e : lex_ )
            for ( const auto &le : e.second )
                if ( le.isSuffixAttached() )
                        suffix.push_back(e.first);

        // sort them based on length desc
        std::sort(suffix.begin(), suffix.end(), sortByStringLength);

        // concat them into a regex fragment
        std::string str;
        for ( const auto &e : suffix ) {
            std::string ee = escapeRegex( e );
            str += "\\B" + ee + "\\b|";
        }

        // remove trailing '|' char
        if (str.length() > 0)
            str.pop_back();

        regexSuffix_ = str;
    }

    return regexSuffix_;
}


// PRIVATE methods

static const char* special_chars_regex = "([-.+*~$()\\[\\]\\\\|?])";
static const char* special_chars_replace = "\\\\$1";


std::string Lexicon::escapeRegex( const std::string &str ) {

    boost::regex re(special_chars_regex);
    boost::regex re2("(\\s+)");

    // escape all characters that have special meaning in a regex
    // .+*-~^$()[]\|?
    std::string tmp = boost::regex_replace(str, re, special_chars_replace);
    std::string outstr = boost::regex_replace(tmp, re2, "\\\\s+");

    return outstr;
}
