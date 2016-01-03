/*HEADER
*HEADER*/

#ifndef LEXICON_H
#define LEXICON_H

#include <map>
#include <deque>
#include <string>
#include <iostream>

#include "inclass.h"
#include "lexentry.h"

class Lexicon
{
protected:

public:
    // constructors
    Lexicon();
    Lexicon(std::string name);
    Lexicon(std::string name, std::string file);

    // getters
    std::string name() const { return name_; };
    InClass::Lang lang() const { return lang_; };
    std::string langAsString() const { return InClass::langToString(lang_); };
    std::string langAsName() const { return InClass::langToName(lang_); };
    std::string locale() const { return locale_; };

    void dump() const;

    // mutators
    void name( const std::string name ) { name_ = name; };
    void lang( const InClass::Lang lang ) { lang_ = lang; };
    void langAsString( std::string lang );
    void locale( std::string locale ) { locale_ = locale; };

private:
    std::string name_;
    InClass::Lang lang_;
    std::string locale_;
    std::deque<LexEntry> lex_;


};

#endif
