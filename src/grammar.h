/*HEADER*
*HEADER*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <map>
#include <vector>
#include <string>

#include "inclass.h"
#include "outclass.h"
#include "rule.h"

class Grammar
{
public:
/*
    typedef struct {
        std::vector<InClass::Type> inKey_;
    } Key;

    typedef struct {
        std::vector<OutClass::Type> outKey_;
        float score_;
    } Value;
*/

    Grammar( const std::string &file );

    std::string check() const;

    friend std::ostream &operator<<(std::ostream &ss, const Grammar &g);


private:

    std::map<std::string, std::vector<Rule> > rules_;


};

#endif
