/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>

#include "token.h"

Token::Token() {
    text_ = "";
    stdtext_ = "";
    attached_ = InClass::NO;
}


Token::Token(std::string text) {
    text_ = text;
    stdtext_ = "";
    attached_ = InClass::NO;
}


std::ostream &operator<<(std::ostream &ss, const Token &token) {
    std::string tclass = token.tclassAsString();
    std::string attached = token.attachedAsString();

    ss << "Token:" << "\t"
       << token.text_ << "\t"
       << token.stdtext_ << "\t"
       << tclass << "\t"
       << attached;

    return ss;
}
