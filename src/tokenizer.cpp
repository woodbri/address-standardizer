/*HEADER*
*HEADER*/

#include <deque>
#include <string>
#include <boost/tokenizer.hpp>

#include "tokenizer.h"



Tokenizer::Tokenizer() {
    separatorsNoToken_ = " \t";
    separatorsToken_   = "!@#$%^&*()-+={}|[]:\";'<>,.?/";
}


Tokenizer::Tokenizer(std::string separatorsNoToken, std::string separatorsToken) {
    separatorsNoToken_ = separatorsNoToken;
    separatorsToken_   = separatorsToken;
}


std::deque<Token> Tokenizer::getTokens(std::string str) {
    typedef boost::tokenizer<boost::char_separator<char> >
        tokenizer;

    boost::char_separator<char> sep(separatorsNoToken_.c_str(), separatorsToken_.c_str());

    tokenizer tokens(str, sep);
    std::deque<Token> outtokens;
    for (tokenizer::iterator tok_iter = tokens.begin();
         tok_iter != tokens.end(); ++tok_iter) {
        Token tok(*tok_iter);
        tok.tclass(InClass::WORD);
        outtokens.push_back(tok);
    }

    return outtokens;
}

