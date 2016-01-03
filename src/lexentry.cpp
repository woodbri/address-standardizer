/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>
#include <string>

#include "lexentry.h"
#include "inclass.h"

void LexEntry::dump() const {
    std::string type = InClass::typeToString(type_);
    std::string lang = InClass::langToString(lang_);

    std::stringstream ss;
    ss << word_ << "\t"
       << stdword_ << "\t"
       << type << "\t"
       << lang;

    // later we might want to output to a logger instead of cout
    std::cout << "LexEntry: " << ss.str() << "\n";
}


LexEntry::LexEntry()
  : word_(""),
    stdword_(""),
    type_(InClass::BADTOKEN) {
}


LexEntry::LexEntry(const std::string &word, const std::string &stdword, const InClass::Type type, const InClass::Lang lang)
  : word_(word),
    stdword_(stdword),
    type_(type),
    lang_(lang) {
}


LexEntry::LexEntry(const std::string &line)
  : word_(""),
    stdword_(""),
    type_(InClass::BADTOKEN),
    lang_(InClass::UNKNOWN)
{
    std::string in_word;
    std::string in_stdword;
    std::string in_type;
    std::string in_lang;

    std::stringstream buffer(line);
    std::getline(buffer, in_word, '\t');
    buffer >> in_stdword >> in_type >> in_lang;

    set_word( in_word );
    set_stdword( in_stdword );
    set_type( InClass::stringToType( in_type ) );
    set_lang( InClass::stringToLang( in_lang ) );
}

