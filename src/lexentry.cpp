/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>
#include <string>

#include "lexentry.h"
#include "inclass.h"

LexEntry::LexEntry()
  : word_(""),
    stdword_(""),
    type_(InClass::BADTOKEN),
    lang_(InClass::UNKNOWN),
    attached_(InClass::NO) {
}


LexEntry::LexEntry(const std::string &word, const std::string &stdword, const InClass::Type type, const InClass::Lang lang, const InClass::AttachType attached)
  : word_(word),
    stdword_(stdword),
    type_(type),
    lang_(lang),
    attached_(attached) {
}


LexEntry::LexEntry(const std::string &line)
  : word_(""),
    stdword_(""),
    type_(InClass::BADTOKEN),
    lang_(InClass::UNKNOWN),
    attached_(InClass::NO)
{
    std::string in_word;
    std::string in_stdword;
    std::string in_type;
    std::string in_lang;
    std::string in_attached;

    std::stringstream buffer(line);
    std::getline(buffer, in_word, '\t');
    buffer >> in_stdword >> in_type >> in_lang >> in_attached;

    word_ = in_word;
    stdword_ = in_stdword;
    type_ = InClass::asType( in_type );
    lang_ = InClass::asLang( in_lang );
    attached_ = InClass::asAttachType( in_attached );
}

std::ostream &operator<<(std::ostream &ss, const LexEntry &le) {
    std::string type = InClass::asString(le.type_);
    std::string lang = InClass::asString(le.lang_);
    std::string attached = InClass::asString(le.attached_);

    ss << "LexEntry:" << "\t"
       << le.word_ << "\t"
       << le.stdword_ << "\t"
       << type << "\t"
       << lang << "\t"
       << attached;

    return ss;
}


