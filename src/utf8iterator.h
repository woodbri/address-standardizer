

// A code point iterator adapter for C++ strings in UTF-8
// code  and ideas taken from http://www.nubaria.com/en/blog/?p=371
// Posted on May 30, 2011 by Ángel José Riesgo 

#ifndef UTF8ITERATOR_H
#define UTF8ITERATOR_H

#include <string>
#include <iterator>
#include <stdexcept>

typedef std::iterator<std::bidirectional_iterator_tag, char32_t, std::string::difference_type, const char32_t*, const char32_t&> StringItr;

class Utf8Iterator : public std::iterator<std::bidirectional_iterator_tag, char32_t, std::string::difference_type, const char32_t*, const char32_t&>
{
public:
 
    Utf8Iterator(std::string::const_iterator it);
    Utf8Iterator(const Utf8Iterator& source);
    ~Utf8Iterator();

    std::string substr( Utf8Iterator &to );

    Utf8Iterator& operator=(const Utf8Iterator& rhs);
    Utf8Iterator& operator++();
    Utf8Iterator operator++(int);
    Utf8Iterator& operator--();
    Utf8Iterator operator--(int);
    char32_t operator*() const;
    bool operator==(const Utf8Iterator& rhs) const;
    bool operator!=(const Utf8Iterator& rhs) const;
    bool operator==(std::string::iterator rhs) const;
    bool operator==(std::string::const_iterator rhs) const;
    bool operator!=(std::string::iterator rhs) const;
    bool operator!=(std::string::const_iterator rhs) const;
    
private:

    void CalculateCurrentCodePoint() const;
 
private:
 
    std::string::const_iterator mStringIterator;
    mutable char32_t mCurrentValue;
    mutable bool mDirty;

};

// non-class utility functions

std::string CodePointToUtf8( char32_t codePoint );

#endif
