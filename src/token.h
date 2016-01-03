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

    typedef enum {
        NO      = 0,
        PREFIX  = 1,
        SUFFIX  = 2,
        EITHER  = 3
    } AttachType;

    Token();
    Token(std::string text);

    // getters
    std::string text() const { return text_; };
    std::string stdtext() const { return stdtext_; };
    InClass::Type tclass() const { return tclass_; };
    AttachType attached() const { return attached_; };

    std::string attachedToString() const;
    std::string tclassToString() const;

    // mutators
    void text(std::string text) { text_ = text; };
    void stdtext(std::string stdtext) { stdtext_ = stdtext; };
    void tclass(InClass::Type tclass) { tclass_ = tclass; };
    void attached(AttachType attached) { attached_ = attached; };

    void classify(Lexicon lexicon);

    void dump() const;

private:
    std::string text_;
    std::string stdtext_;
    InClass::Type tclass_;
    AttachType attached_;

};

#endif

