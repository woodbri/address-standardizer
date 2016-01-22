/*HEADER*
*HEADER*/

#include <deque>
#include <string>
#include <boost/tokenizer.hpp>

#include "tokenizer.h"


std::deque<Token> Tokenizer::getTokens(std::string str) {

    // first see if we have attached tokens that we need to split apart
    std::string attached = "(" + lex_.attachedRegex() + ")";
    if (attached.length() > 2) {
        boost::regex re( attached, boost::regex::icase );
        const char* replace( " $1 " );
        std::string tmp = boost::regex_replace( str, re, replace );
        str = tmp;
    }

    // build the regex for identifying tokens
    std::string regex = "^\\s*(" + lex_.regex() + "\\d+/\\d+|\\d+|\\<[[:alpha:]]+\\>|\\w+)([-&\\s\\|[:punct:]]+|$)";
    boost::regex expression( regex, boost::regex::icase );

    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;

    std::deque<Token> outtokens;

    // could not get this to work with auto
    std::string::const_iterator start, end;
    start = str.begin();
    end   = str.end();

    while(boost::regex_search(start, end, what, expression, flags)) {

        // fetch and create the token for the word or phrase
        if (what[1].first < what[1].second) {
            Token tok( std::string(what[1].first, what[1].second) );
            // token might classify as multiple types
            // or none, in which case make it a word
            lex_.classify(tok, InClass::WORD);
            if (not tok.isInClass(filter_))
                outtokens.push_back(tok);
        }

        // create a token for the punctuation
        if (what[2].first < what[2].second) {
            Token punct( std::string(what[2].first, what[2].second) );
            lex_.classify(punct, InClass::PUNCT);
            if (not punct.isInClass(filter_))
                outtokens.push_back(punct);
        }

        // update search position
        start = what[0].second;

        // update flags
        flags |= boost::match_not_bob;
    }

    return outtokens;
}

