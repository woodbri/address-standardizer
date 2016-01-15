/*HEADER*
*HEADER*/

#include <iostream>
#include <sstream>
#include <string>

#include "lexentry.h"
#include "inclass.h"


bool LexEntry::isPrefixAttached() const {
    auto it = attached_.find( InClass::PREFIX );
    if (it == attached_.end())
        return false;
    else
        return true;
}


bool LexEntry::isSuffixAttached() const {
    auto it = attached_.find( InClass::SUFFIX );
    if (it == attached_.end())
        return false;
    else
        return true;
}


bool LexEntry::isDettached() const {
    auto it = attached_.find( InClass::DETACH );
    if (it == attached_.end())
        return false;
    else
        return true;
}


LexEntry::LexEntry()
  : word_(""),
    stdword_("") {
}


LexEntry::LexEntry(const std::string &in_word, const std::string &in_stdword, const std::string &in_type, const std::string &in_attached)
  : word_(in_word),
    stdword_(in_stdword) {
    type(InClass::asType(in_type));
    attached(InClass::asAttachType(in_attached));
}


LexEntry::LexEntry(const std::string &line)
  : word_(""),
    stdword_("")
{
    std::string in_word;
    std::string in_stdword;
    std::string in_type;
    std::string in_attached;

    std::stringstream buffer(line);
    std::getline(buffer, in_word, '\t');
    if (in_word == "LEXENTRY:")
        buffer >> in_word;
    buffer >> in_stdword >> in_type >> in_attached;

    word_     = in_word;
    stdword_  = in_stdword;
    type_     = InClass::asType( in_type );
    attached_ = InClass::asAttachType( in_attached );
}

std::ostream &operator<<(std::ostream &ss, const LexEntry &le) {
    std::string type = InClass::asString(le.type_);
    std::string attached = InClass::asString(le.attached_);

    ss << "LEXENTRY:" << "\t"
       << le.word_ << "\t"
       << le.stdword_ << "\t";

    ss << InClass::asString(le.type()) << "\t";
    ss << InClass::asString(le.attached());

    return ss;
}


