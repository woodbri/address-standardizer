/*HEADER*
*HEADER*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <set>

#include "inclass.h"
#include "lexicon.h"
#include "token.h"

class Token {

public:

    Token();
    Token(std::string text);

    // getters
    std::string text() const { return text_; };
    std::string stdtext() const { return stdtext_; };
    std::set<InClass::Type> tclass() const { return tclass_; };
    InClass::AttachType attached() const { return attached_; };

    std::string attachedAsString() const { return InClass::asString( attached_ ); };
    std::string tclassAsString() const { return InClass::asString( tclass_ ); };

    // mutators
    void text(std::string text) { text_ = text; };
    void stdtext(std::string stdtext) { stdtext_ = stdtext; };
    void tclass(InClass::Type tclass) { tclass_.insert( tclass ); };
    void attached(InClass::AttachType attached) { attached_ = attached; };

    // operators

    friend std::ostream &operator<<(std::ostream &ss, const Token &token);


private:
    std::string text_;
    std::string stdtext_;
    std::set<InClass::Type> tclass_;
    InClass::AttachType attached_;

};

#endif

