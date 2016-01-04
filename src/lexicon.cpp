/*HEADER
*HEADER*/

#include <iostream>
#include <sstream>
#include <string>

#include "lexentry.h"
#include "inclass.h"
#include "lexicon.h"

// constructors

Lexicon::Lexicon() {
    name_ = "";
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();
}


Lexicon::Lexicon(std::string name) {
    name_ = name;
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();
}


Lexicon::Lexicon(std::string name, std::string file) {
    name_ = name;
    lang_ = InClass::UNKNOWN;
    locale_ = "";
    lex_.clear();

    // TODO read file and load lexicon
}

// getters


// operators

std::ostream &operator<<(std::ostream &ss, const Lexicon &lex) {
    ss << "Lexicon:" << "\t"
       << lex.name_ << "\t"
       << lex.langAsString() << "\t"
       << lex.locale_ << "\t"
       << lex.lex_.size() << "\n";

    for (auto it = lex.lex_.begin(); it != lex.lex_.end(); it++)
        ss << *it << "\n";

    return ss;
}


// mutators

void Lexicon::langAsString( std::string lang ) {
    lang_ = InClass::asLang( lang );
}

