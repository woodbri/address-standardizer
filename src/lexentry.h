/*HEADER
*HEADER*/
#ifndef LEXENTRY_H
#define LEXENTRY_H

#include <string>

#include "inclass.h"

class LexEntry
{
public:
    /** @name  accessors */
    ///@{
    std::string word() const { return word_; };
    std::string stdword() const { return stdword_; };
    InClass::Type type() const { return type_; };
    InClass::Lang lang() const { return lang_; };
    InClass::AttachType attached() const { return attached_; };
    bool isAttached() const { return attached_ != InClass::NO; };
    ///@}

    /** @name mutators */
    ///@{
    void set(const std::string &word, const std::string &stdword, const InClass::Type &type);
    void word(const std::string &word) { word_=word; };
    void stdword(const std::string &stdword) { stdword_=stdword; };
    void type(const InClass::Type type) { type_=type; };
    void lang(const InClass::Lang lang) { lang_=lang; };
    void attached(const InClass::AttachType attached) { attached_=attached; };
    ///@}

    /** @name constructors */
    ///@{
    LexEntry();
    LexEntry(const std::string &word, const std::string &stdword, const InClass::Type type, const InClass::Lang lang, const InClass::AttachType attached);
    explicit LexEntry(const std::string &line);
    ///@}

    /** @name operators */
    ///@{
    friend std::ostream &operator<<(std::ostream &ss, const LexEntry &le);

    inline bool operator==(const LexEntry &rhs) const {
        return word_==rhs.word_ and stdword_==rhs.stdword_
            and type_==rhs.type_ and lang_==rhs.lang_;
    }


    // ~LexEntry() {};

private:
    std::string word_;
    std::string stdword_;
    InClass::Type type_;
    InClass::Lang lang_;
    InClass::AttachType attached_;

};

#endif
