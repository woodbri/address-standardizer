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


void Token::dump() const {
    std::string tclass = tclassAsString();
    std::string attached = attachedAsString();

    std::stringstream ss;
    ss << text_ << "\t"
       << stdtext_ << "\t"
       << tclass << "\t"
       << attached;

    std::cout << "Token: " << ss.str() << "\n";
}

