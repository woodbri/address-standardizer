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
    std::string in_word;
    std::string in_stdword;
    std::string in_tclass;
    std::string in_attached;

    std::stringstream buffer(text);
    std::getline(buffer, in_word, '\t');
    buffer >> in_stdword >> in_tclass >> in_attached;

    text_ = in_word;
    stdtext_ = in_stdword;
    attached_ = InClass::asAttachType(in_attached);

    std::stringstream buffer2(in_tclass);
    while (true) {
        std::getline(buffer2, in_word, ',');
        if (in_word.length() > 0)
            tclass(InClass::asType(in_word));
        if (buffer2.eof())
            break;
    }
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
