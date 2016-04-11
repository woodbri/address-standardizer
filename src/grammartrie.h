

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

class GrammarTrie;

typedef std::map<InClass::Type, GrammarTrie*> GTChildMap;

class GrammarTrie {

public:

    explicit GrammarTrie( Rule *data=NULL ) : _data(data) {};
    ~GrammarTrie() {
        delete _data;
        for ( auto &child : _children )
            delete child.second;
    };

    void serialize( std::ostream &os ) const;
    void deSerialize( std::istream &is );

    vInClass getPatternFromRule( Rule &data ) const;
    void addPattern( const vInClass &pattern, Rule *data );
    Rule *match( const vInClass &pattern ) const;
    void getPatterns( PatternSet &patterns, vInClass patternSoFar=vInClass() ) const;
    void getPatternsStartingWith( vInClass &prefix, PatternSet &patterns, vInClass patternSoFar=vInClass() ) const;


private:

    GTChildMap _children;
    Rule *_data;

};

#endif
