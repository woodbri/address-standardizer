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


void Lexicon::dump() const {
    std::stringstream ss;
    ss << name_ << "\t"
       << langAsString() << "\t"
       << locale_;

    // later we might want to output to a logger instead of cout
    std::cout << "Lexicon: " << ss.str() << "\n";

    // TODO dump the lexicon entries
    std::cout << "Lexicon Entries (TODO)" << "\n";
}


// mutators

void Lexicon::langAsString( std::string lang ) {
    lang_ = InClass::stringToLang( lang );
}


