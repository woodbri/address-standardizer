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
    std::getline(buffer, in_stdword, '\t');
    std::getline(buffer, in_inclass, '\t');
    std::getline(buffer, in_attached, '\t');

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


InClass::Type Token::in(int pos) const {
    auto it = inclass_.begin();
    for (int i=0; i<pos; ++i)
        it++;
    return *it;
}


std::vector< std::vector<InClass::Type> > Token::enumerate( std::vector<Token> tokens ) {

    std::set< std::vector<InClass::Type> > ulist;

    // count the number of possible combinations
    int cnt = 1;
    for (const auto &t : tokens)
        cnt *= t.inSize();

    // enumerate all the combinations and insert them into a set
    // so we can remove duplicates
    for (int i=0; i<cnt; ++i) {
        std::vector<InClass::Type> one;
        for ( const auto &t : tokens )
            one.push_back( t.in( i % t.inSize() ) );
        ulist.insert( one );
    }

    // copy the unique set of enumerated combinations
    // into a vector and return it
    std::vector< std::vector<InClass::Type> > list;
    list.clear();
    list.reserve( ulist.size() );

    for (const auto &e : ulist)
        list.push_back( e );

    return list;
}
