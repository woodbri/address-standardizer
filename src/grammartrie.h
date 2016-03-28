

#ifndef GRAMMARTRIE_H
#define GRAMMARTRIE_H

#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "inclass.h"
#include "rule.h"

typedef std::vector<InClass::Type> vInClass;
typedef std::set<vInClass> PatternSet;
typedef std::vector<Rule*> Data;

class GrammarTrie;

typedef std::map<InClass::Type, GrammarTrie*> ChildMap;

class GrammarTrie {

public:

    explicit GrammarTrie( Data *data=NULL ) : _data(data) {};
    ~GrammarTrie() {
        for ( auto &child : _children )
            delete child.second;
    };

    vInClass getPatternFromData( Data *data );
    void addPattern( const vInClass &pattern, Data *data );
    Data *match( const vInClass &pattern ) const;
    void getPatterns( PatternSet &patterns, vInClass patternSoFar=vInClass() ) const;
    void getPatternsStartingWith( vInClass &prefix, PatternSet &patterns, vInClass patternSoFar=vInClass() ) const;


private:

    ChildMap _children;
    Data *_data;

};

#endif
