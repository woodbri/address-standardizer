/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>

#include "token.h"

Token::Token() {
    text_ = "";
    stdtext_ = "";
    tclass_ = InClass::BADTOKEN;
    attached_ = InClass::NO;
}


Token::Token(std::string text) {
    text_ = text;
    stdtext_ = "";
    tclass_ = InClass::BADTOKEN;
    attached_ = InClass::NO;
}


std::string Token::attachedAsString() const {
    switch (attached_) {
        case InClass::NO:     return "NO";     break;
        case InClass::PREFIX: return "PREFIX"; break;
        case InClass::SUFFIX: return "SUFFIX"; break;
        case InClass::EITHER: return "EITHER"; break;
    }
    return "NO";
}


std::string Token::tclassAsString() const {
    return InClass::asString(tclass_);
}


void Token::classify(Lexicon lexicon) {
    // see if token is in lexicon
    // is it a number, is it 4 or 5 digits
    // is it a mixed
    // is it a fraction
    // is it punctuation
    // check for attached terms
    // else its a word
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
