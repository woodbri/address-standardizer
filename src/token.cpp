/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>

#include "token.h"

Token::Token() {
    text_ = "";
    stdtext_ = "";
    tclass_ = InClass::BADTOKEN;
    attached_ = NO;
}


Token::Token(std::string text) {
    text_ = text;
    stdtext_ = "";
    tclass_ = InClass::BADTOKEN;
    attached_ = NO;
}


std::string Token::attachedAsString() const {
    switch (attached_) {
        case NO:     return "NO";     break;
        case PREFIX: return "PREFIX"; break;
        case SUFFIX: return "SUFFIX"; break;
        case EITHER: return "EITHER"; break;
    }
    return "NO";
}


std::string Token::tclassAsString() const {
    return InClass::asString(tclass_);
}


void Token::classify(Lexicon lexicon) {
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
