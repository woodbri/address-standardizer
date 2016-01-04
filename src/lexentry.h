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
    ///@}

    /** @name mutators */
    ///@{
    void set(const std::string &word, const std::string &stdword, const InClass::Type &type);
    void set_word(const std::string &word) { word_=word; };
    void set_stdword(const std::string &stdword) { stdword_=stdword; };
    void set_type(const InClass::Type type) { type_=type; };
    void set_lang(const InClass::Lang lang) { lang_=lang; };
    ///@}

    /** @name constructors */
    ///@{
    LexEntry();
    LexEntry(const std::string &word, const std::string &stdword, const InClass::Type type, const InClass::Lang lang);
    explicit LexEntry(const std::string &line);
    ///@}

    /** @name operators */
    ///@{
    friend std::ostream &operator<<(std::ostream &ss, const LexEntry &le);

    // ~LexEntry() {};

private:
    std::string word_;
    std::string stdword_;
    InClass::Type type_;
    InClass::Lang lang_;

};

#endif
