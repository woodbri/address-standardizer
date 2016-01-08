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
    attached_(InClass::NO) {
}


LexEntry::LexEntry(const std::string &in_word, const std::string &in_stdword, const InClass::Type in_type, const InClass::AttachType in_attached)
  : word_(in_word),
    stdword_(in_stdword),
    attached_(in_attached) {
    type(in_type);
}


LexEntry::LexEntry(const std::string &line)
  : word_(""),
    stdword_(""),
    attached_(InClass::NO)
{
    std::string in_word;
    std::string in_stdword;
    std::string in_type;
    std::string in_attached;

    std::stringstream buffer(line);
    std::getline(buffer, in_word, '\t');
    if (in_word == "LexEntry:")
        buffer >> in_word;
    buffer >> in_stdword >> in_type >> in_attached;

    word_ = in_word;
    stdword_ = in_stdword;
    std::stringstream buffer2(in_type);
    while (true) {
        std::getline(buffer2, in_type, ',');
        if (in_type.length() > 0)
            type(InClass::asType( in_type ));
        if (buffer2.eof())
            break;
    }
    attached_ = InClass::asAttachType( in_attached );
}

std::ostream &operator<<(std::ostream &ss, const LexEntry &le) {
    std::string type = InClass::asString(le.type_);
    std::string attached = InClass::asString(le.attached_);

    ss << "LexEntry:" << "\t"
       << le.word_ << "\t"
       << le.stdword_ << "\t";

    std::set<InClass::Type> typ = le.type();
    for (auto it=typ.begin(); it!=typ.end(); it++) {
        if (it!=typ.begin())
            ss << ",";
        ss << InClass::asString(*it);
    }
    ss << "\t" << attached;

    return ss;
}


