
#include <boost/lexical_cast.hpp>
#include "grammartrie.h"

void GrammarTrie::serialize( std::ostream &os ) const {
    if ( _data != NULL ) {
        //os << "[ " << *_data << "\n";
        os << "[ ";
        for ( long unsigned int pos = 0; pos < _data->inSize(); ++pos )
            os << _data->in( pos ) << " ";
        os << "-1 ";
        for ( long unsigned int pos = 0; pos < _data->outSize(); ++pos )
            os << _data->out( pos ) << " ";
        os << "-1 " << _data->score() << "\n";
    }

    for ( const auto &it : _children ) {
        os << it.first << "\n";
        it.second->serialize( os );
    }
    os << "]\n";
}


void GrammarTrie::deSerialize( std::istream &is ) {
    std::string line;
    while ( is ) {
        std::getline( is, line );
        if ( line.length() > 0 ) {
            if ( line[0] == ']' )
                return;
            else if ( line[0] == '[' )
                _data = new Rule( line );
            else {
                InClass::Type key( InClass::asType( boost::lexical_cast<int>( line ) ) );
                GrammarTrie * tmp = new GrammarTrie( NULL );
                tmp->deSerialize( is );
                _children[ key ] = tmp;
            }
        }
    }
}


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


