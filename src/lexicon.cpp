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


Lexicon::Lexicon(std::string name) :
    name_(name), lang_(InClass::UNKNOWN), locale_("")
{}


Lexicon::Lexicon(std::string name, std::istream &is ) :
    name_(name), lang_(InClass::UNKNOWN), locale_("")
{
    initialize( is );
}


Lexicon::Lexicon(std::string name, std::string file) :
    name_(name), lang_(InClass::UNKNOWN), locale_("")
{
    std::ifstream ifs;
    ifs.open( file.c_str(), std::ifstream::in);
    initialize( ifs );
    ifs.close();
}


void Lexicon::initialize( std::istream &is ) {
    std::string line;
    unsigned long int cnt = 0;

    std::getline( is, line );
    // remove UTF8 BOM if one exists
    if (line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF')
        line = line.substr(4);
    ++cnt;

    // read the header and skip any
    while (is and line.substr(0,8) != "LEXICON:") {
        if (line.substr(0,9) == "LEXENTRY:") {
            throw std::runtime_error("Lexicon-LexEntry-Before-Lexicon");
        }
        std::getline( is, line );
        ++cnt;
    }

    if ( not is )
        throw std::runtime_error("Lexicon-Invalid-File-Format");

    // echo out the read line indented for debugging
    //std::cout << "\t" << line << "\n";

    std::stringstream buffer(line);
    std::string term;
    std::getline(buffer, term, '\t');
    ++cnt;
    if (term != "LEXICON:" and term != "Lexicon:") {
        throw std::runtime_error("Lexicon-Invalid-Data-Format: " + std::to_string(cnt) + ": " + line);
    }

    std::getline(buffer, term, '\t');   // name (ignored)
    std::getline(buffer, term, '\t');   // lang

    lang_ = InClass::asLang( term );        // set the language
    std::getline(buffer, locale_, '\t');    // set the locale
                                            // ignore the count of keys

    UErrorCode errorCode;

    // read in the lexicon entries
    while ( is ) {
        std::getline( is, line );
        ++cnt;
        //std::cout << "\t" << line << "\n";

        // replace multiple space chars with a single space
        // but don't touch tab chars
        auto re = boost::make_u32regex( "( +)" );
        const char* replace = " ";
        std::string tmp = boost::u32regex_replace(line, re, replace);
        line = tmp;

        if (line.length() == 0) continue;

        if ( line.compare(0, 9, "LEXENTRY:") == 0 or
             line.compare(0, 9, "LexEntry:") == 0 ) {
            line = Utils::normalizeUTF8( line, errorCode );
            if (locale_ != "") {
                line = Utils::upperCaseUTF8( line, locale_ );
            }
            LexEntry le( line );
            insert( le );
        }
        else {
            throw std::runtime_error("Lexicon-Invalid-LexEntry: "+ std::to_string(cnt) + ": " + line);
        }
    }
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


void Lexicon::standardize( Token& token ) {
    std::string key = token.text();
    std::vector<LexEntry> entries = find( key );

    // if the token has been reclassified
    // then only that InClass will be set and .begin() will be it
    for ( const auto &entry : entries ) {
        if ( entry.isInClass( *(token.inclass().begin()) ) ) {
            token.stdtext( entry.stdword() );
            break;
        }
    }

    // if an entry was not found for token.inclass()
    // we use the token text as the standard text
    if ( token.stdtext().size() == 0 )
        token.stdtext( token.text() );
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

    if ( not token.isInClassEmpty() ) {
        token.inLex(true);
        return;
    }

    token.inLex(false);

    auto num_exp    = boost::make_u32regex("^\\d+$");
    auto float_exp  = boost::make_u32regex("^\\d+[,\\.]\\d+$");
    auto word_exp   = boost::make_u32regex("^\\w+$");
    auto has_digit  = boost::make_u32regex("^.*\\d.*$");
    auto dash_exp   = boost::make_u32regex("^[-]+$");
    auto emdash_exp = boost::make_u32regex("^\xe2\x80\x94$");
    auto punct_exp  = boost::make_u32regex("^[\\|[:punct:]]+$");
    auto space_exp  = boost::make_u32regex("^\\s+$");
    auto fract_exp  = boost::make_u32regex("^\\d+/\\d+$");
    auto pch_exp    = boost::make_u32regex("^[A-Z]{1,2}\\d{1,2}[A-Z]{0,1}$", boost::regex::icase);
    auto pct_exp    = boost::make_u32regex("^(\\d[A-Z]\\d|\\d[A-Z]{2})$", boost::regex::icase);

    // is it a number
    if (boost::u32regex_match( text, float_exp )) {
        token.inclass( InClass::NUMBER );
        auto pos = text.find_first_of(",");
        if ( pos != std::string::npos ) {
            text[pos] = '.';
            token.text(text);
        }
    }
    else if (boost::u32regex_match( text, num_exp )) {
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
    else if (boost::u32regex_match( text, fract_exp )) {
        token.inclass( InClass::FRACT );
    }
    // is it pch
    else if (boost::u32regex_match( text, pch_exp )) {
        token.inclass( InClass::MIXED );
        token.inclass( InClass::PCH );
    }
    // is it pct
    else if (boost::u32regex_match( text, pct_exp )) {
        token.inclass( InClass::MIXED );
        token.inclass( InClass::PCT );
    }
    // is it alpha
    // is mixed alpha and digit
    else if (boost::u32regex_match( text, word_exp )) {
        if (boost::u32regex_match( text, has_digit ))
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
    else if (boost::u32regex_match( text, emdash_exp )) {
        token.inclass( InClass::EMDASH );
    }
    // is it dash
    else if (boost::u32regex_match( text, dash_exp )) {
        token.inclass( InClass::DASH );
    }
    // is it ampersand
    else if (text == "&") {
        token.inclass( InClass::AMPERS );
    }
    // is it whitespace
    else if (boost::u32regex_match( text, space_exp )) {
        token.inclass( InClass::SPACE );
    }
    // is it punct
    else if (boost::u32regex_match( text, punct_exp )) {
        token.inclass( InClass::PUNCT );
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

    // set cached regex strings to empty
    // so it will get regenerated
    regex_.clear();
    regexPrefix_.clear();
    regexSuffix_.clear();
}


void Lexicon::remove( const LexEntry &le ) {
    
    // get the map key we want to find
    std::string key = le.word();

    // fetch the entry from the lexicon
    // we get an empty container if it is not found
    std::vector<LexEntry> entries = find( key );
    if (entries.size() == 0)
        return;

    // see if it is already here and erase it if it is
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

        // sort them based on longest to shortest
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

        // sort them based on longest to shortest
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

        // sort them based on longest to shortest
        std::sort(suffix.begin(), suffix.end(), sortByStringLength);

        // concat them into a regex fragment
        std::string str;
        for ( const auto &e : suffix ) {
            std::string ee = escapeRegex( e );
            str += ee + "|";
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

    auto re  = boost::make_u32regex( special_chars_regex );
    auto re2 = boost::make_u32regex( "(\\s+)" );

    // escape all characters that have special meaning in a regex
    // .+*-~^$()[]\|?
    std::string tmp = boost::u32regex_replace(str, re, special_chars_replace);
    std::string outstr = boost::u32regex_replace(tmp, re2, "\\\\s+");

    return outstr;
}


