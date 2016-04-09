

#include "grammartrie.h"

vInClass GrammarTrie::getPatternFromRule( Rule &data ) const {
    vInClass patt;
    if ( data.inSize() == 0 )
        return patt;

    for ( auto in = data.begin(); in != data.end(); ++in )
        patt.push_back( *in );

    return patt;
}


void GrammarTrie::addPattern( const vInClass &pattern, Rule *data ) {
    if ( pattern.size() > 0 ) {
        vInClass tail( pattern.begin()+1, pattern.end() );
        if ( _children[ pattern[0] ] ) {
            if ( pattern.size() == 1 )
                _children[ pattern[0] ]->_data = data;
            else
                _children[ pattern[0] ]->addPattern( tail, data );
        }
        else {
            GrammarTrie * tmp = new GrammarTrie( pattern.size() == 1 ? data : NULL );
            tmp->addPattern( tail, data );
            _children[ pattern[0] ] = tmp;
        }
    }
}


Rule *GrammarTrie::match( const vInClass &pattern ) const {
    if ( pattern.size() == 0 )
        return _data;

    vInClass tail( pattern );
    const GrammarTrie *tmp = this;

    while ( tail.size() > 0 ) {
        if ( tmp->_children.find( tail[0] ) != tmp->_children.end() ) {
            tmp = tmp->_children.find( tail[0] )->second;
            tail = vInClass( tail.begin()+1, tail.end() );
        }
        else {
            return NULL;
        }
    }
    return tmp->_data;
}


void GrammarTrie::getPatterns( PatternSet &patterns, vInClass patternSoFar ) const {
    if ( _data != NULL )
        patterns.insert( patternSoFar );

    for ( const auto &it : _children ) {
        vInClass tmp( patternSoFar );
        tmp.push_back( it.first );
        if ( it.second and it.second->_data != NULL )
            patterns.insert( tmp );
        it.second->getPatterns( patterns, tmp );
    }

}


void GrammarTrie::getPatternsStartingWith( vInClass &prefix, PatternSet &patterns, vInClass patternSoFar ) const {
    if ( prefix.size() == 0 ) {
        getPatterns( patterns, patternSoFar );
        return;
    }

    vInClass tail( prefix.begin()+1, prefix.end() );
    if ( _children.find( prefix[0] ) != _children.end() ) {
        GrammarTrie *tmp = _children.find( prefix[0] )->second;
        vInClass npsf( patternSoFar );
        npsf.push_back( prefix[0] );
        tmp->getPatternsStartingWith( tail, patterns, npsf );
    }
}


