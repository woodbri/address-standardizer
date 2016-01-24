/*HEADER
*HEADER*/
#ifndef LEXENTRY_H
#define LEXENTRY_H

#include <string>
#include <set>

#include "inclass.h"

class LexEntry
{
public:
    /** @name  accessors */
    ///@{
    std::string word() const { return word_; };
    std::string stdword() const { return stdword_; };
    std::set<InClass::Type> type() const { return type_; };
    std::set<InClass::AttachType> attached() const { return attached_; };
    bool isPrefix() const;
    bool isSuffix() const;
    bool isAttached() const;
    bool isDetached() const;
    bool isPrefixAttached() const;
    bool isSuffixAttached() const;
    ///@}

    /** @name mutators */
    ///@{
    void set(const std::string &word, const std::string &stdword, const InClass::Type &type);
    void word(const std::string &word) { word_=word; };
    void stdword(const std::string &stdword) { stdword_=stdword; };
    void type(const InClass::Type &type) { type_.insert(type); };
    void type(const std::set<InClass::Type> &type) { type_ = type; };
    void attached(const std::set<InClass::AttachType> &attached) { attached_=attached; };
    ///@}

    /** @name constructors */
    ///@{
    LexEntry();
    LexEntry(const std::string &word, const std::string &stdword, const std::string &type, const std::string &attached);
    explicit LexEntry(const std::string &line);
    ///@}

    /** @name operators */
    ///@{
    friend std::ostream &operator<<(std::ostream &ss, const LexEntry &le);

    inline bool operator==(const LexEntry &rhs) const {
        return word_==rhs.word_ and stdword_==rhs.stdword_
            and type_==rhs.type_;
    }
    ///@}


    // ~LexEntry() {};

private:
    std::string word_;
    std::string stdword_;
    std::set<InClass::Type> type_;
    std::set<InClass::AttachType> attached_;

};

#endif
