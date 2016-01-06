/*HEADER*
*HEADER*/

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

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
    InClass::Type tclass() const { return tclass_; };
    InClass::AttachType attached() const { return attached_; };

    std::string attachedAsString() const;
    std::string tclassAsString() const;

    // mutators
    void text(std::string text) { text_ = text; };
    void stdtext(std::string stdtext) { stdtext_ = stdtext; };
    void tclass(InClass::Type tclass) { tclass_ = tclass; };
    void attached(InClass::AttachType attached) { attached_ = attached; };

    void classify(Lexicon lexicon);

    // operators

    friend std::ostream &operator<<(std::ostream &ss, const Token &token);


private:
    std::string text_;
    std::string stdtext_;
    InClass::Type tclass_;
    InClass::AttachType attached_;

};

#endif

