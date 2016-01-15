/*HEADER
*HEADER*/

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
        std::cout << "\t" << line << "\n";
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
            std::cout << "\t" << line << "\n";
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


std::deque<LexEntry> Lexicon::find( const std::string key ) {

    std::deque<LexEntry> empty;

    auto it = lex_.find( key );
    if ( it != lex_.end() )
        return (*it).second;
    else
        return empty;
}


bool Lexicon::isInAttached( const std::string word ) const {

    for ( const auto &e :attached_ )
        if ( e == word ) return true;

    return false;
}


// operators

std::ostream &operator<<(std::ostream &ss, const Lexicon &lex) {
    ss << "LEXICON:" << "\t"
       << lex.name_ << "\t"
       << lex.langAsString() << "\t"
       << lex.locale_ << "\t"
       << lex.lex_.size() << "\n";

    // each lexicon entry is a deque of LexEntry objects
    for (const auto &l  : lex.lex_)
        for (const auto &d : l.second)
            ss << d << "\n";

    ss << "LEXICON.ATTACHED:";

    for (const auto &e : lex.attached_)
        ss << "\t" << e;

    ss << "\n";

    return ss;
}


void Lexicon::classify( Token& token, InClass::Type typ ) {
    
    // fetch the entry from the lexicon
    // we get an empty container if it is not found
    std::string text = token.text();
    std::deque<LexEntry> entries = find( text );

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
    boost::regex punct_exp("^[-&\\s\\|[:punct:]]+$");
    boost::regex space_exp("^\\s+$");
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
    std::deque<LexEntry> entries = find( key );

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
    std::deque<LexEntry> entries = find( key );

    // see if it is already here and do nothing if it is
    for( auto entry=entries.begin(); entry!=entries.end(); entry++ ) {
        if ( *entry == le ) {
            entries.erase( entry );
            if ( le.isAttached() and not isInAttached( le.word() ) ) {
                attached_.push_back( le.word() );
            }

            if ( entries.size() == 0 ) {
                // remove the key from the lexicon
                auto it = lex_.find( key );
                if ( it != lex_.end() )
                    lex_.erase( it );
            }
            else {
                // save it back in the lexicon
                lex_[key] = entries;
            }
        }
    }

    // set cached regex string to empty
    // so it will get regenerated
    regex_ = "";
}

bool sortByStringLength(std::string a, std::string b) {
    return a.length() > b.length();
}

static const char* special_chars_regex = "([-.+*~$()\\[\\]\\\\|?])";
static const char* special_chars_replace = "\\\\$1";

std::string Lexicon::regex() {

    boost::regex re(special_chars_regex);

    // if the regex string is empty, regenerate it
    if (regex_.length() == 0) {
        // collect all lexicon keys in deque
        std::deque<std::string> keys;
        for ( const auto &e : lex_ )
            keys.push_back( e.first );

        // sort them based on length desc
        std::sort(keys.begin(), keys.end(), sortByStringLength);

        // concat them into a regex fragment
        std::string str;
        for ( const auto &key : keys ) {
            // escape all characters that have special meaning in a regex
            // .+*-~^$()[]\|?
            std::string outkey = boost::regex_replace(key, re, special_chars_replace);

            str += outkey + "|";
        }

        regex_ = str;
    }
    
    return regex_;
}


