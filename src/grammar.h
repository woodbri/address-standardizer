/*HEADER*
*HEADER*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <map>
#include <vector>
#include <string>

#include "inclass.h"
#include "outclass.h"

class Grammar
{
public:

    typedef struct {
        std::vector<InClass::Type> inKey_;
    } Key;

    typedef struct {
        std::vector<OutClass::Type> outKey_;
        float score_;
    } Value;

    typedef struct {
        std::vector<std::string> meta;
        Key key;
        Value value;
    } Rule;

    Grammar( const std::string &file );

    friend std::ostream &operator<<(std::ostream &ss, const Grammar &g);


private:

    std::map<std::string, std::vector<Rule> > rules_;


};

#endif
