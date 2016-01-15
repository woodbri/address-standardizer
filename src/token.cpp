/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>

#include "token.h"

Token::Token() {
    text_ = "";
    stdtext_ = "";
}


Token::Token(std::string text) {
    std::string in_word;
    std::string in_stdword;
    std::string in_inclass;
    std::string in_attached;

    std::stringstream buffer(text);
    std::getline(buffer, in_word, '\t');
    if (in_word == "TOKEN:")
        std::getline(buffer, in_word, '\t');
    buffer >> in_stdword >> in_inclass >> in_attached;

    text_ = in_word;
    stdtext_ = in_stdword;
    attached_ = InClass::asAttachType(in_attached);

    std::stringstream buffer2(in_inclass);
    while (true) {
        std::getline(buffer2, in_word, ',');
        if (in_word.length() > 0)
            inclass(InClass::asType(in_word));
        if (buffer2.eof())
            break;
    }
}


std::ostream &operator<<(std::ostream &ss, const Token &token) {
    std::string inclass = token.inclassAsString();
    std::string attached = token.attachedAsString();
    std::string outclass = token.outclassAsString();

    ss << "TOKEN:" << "\t"
       << token.text_ << "\t"
       << token.stdtext_ << "\t"
       << inclass << "\t"
       << outclass << "\t"
       << attached;

    return ss;
}


bool Token::isInClass( std::set<InClass::Type> test ) const {
    for ( const auto &e : test ) {
        if (inclass_.find( e ) != inclass_.end())
            return true;
    }
    return false;
}


