/*HEADER*
*HEADER*/

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <deque>
#include <boost/tokenizer.hpp>

#include "token.h"

class Tokenizer {

public:
    Tokenizer();
    Tokenizer(std::string separatorsNoToken, std::string separatorsToken);
    std::deque<Token> getTokens(std::string str);

private:
    std::string separatorsNoToken_;
    std::string separatorsToken_;

};

#endif

