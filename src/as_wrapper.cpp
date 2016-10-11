/**ADDRESS_STANDARDIZER***************************************************
 *
 * Address Standardizer
 *      A collection of C++ classes for parsing street addresses
 *      and standardizing them for the purpose of Geocoding.
 *
 * Copyright 2016 Stephen Woodbridge <woodbri@imaptools.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file LICENSE for details.
 *
 ***************************************************ADDRESS_STANDARDIZER**/


#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/archive_exception.hpp>

#include "utils.h"
#include "token.h"
#include "lexicon.h"
#include "tokenizer.h"
#include "grammar.h"
#include "search.h"
#include "md5.h"

#include "address_standardizer.h"


STDADDR *standardize_addr( char *address_in, Grammar & grammar, Lexicon & lexicon, char *locale_in, char *filter_in, char **err_msg);



STDADDR *std_standardize_ptrs( char *address_in, void *grammar_ptr, void *lexicon_ptr, char *locale_in, char *filter_in, char **err_msg)
{
    return standardize_addr( address_in,
                             *(static_cast<Grammar*>( grammar_ptr )),
                             *(static_cast<Lexicon*>( lexicon_ptr )),
                             locale_in, filter_in, err_msg );
}



STDADDR *std_standardize( char *address_in, char *grammar_in, char *lexicon_in, char *locale_in, char *filter_in, char **err_msg)
{
    try {

        std::string s( grammar_in );
        std::istringstream iss( s );
        Grammar grammar( iss );

        s = lexicon_in;
        iss.clear();
        iss.str( s );
        Lexicon lexicon;
        try {
            boost::archive::text_iarchive ia(iss);
            ia >> lexicon;
        }
        catch (std::runtime_error& e) {
            if ( e.what() == std::string("Re-compile-lexicon"))
                throw;
            iss.clear();
            iss.str( s );
            lexicon.name( "query-lex" );
            lexicon.initialize( iss );
        }
        catch ( ... ) {
            iss.clear();
            iss.str( s );
            lexicon.name( "query-lex" );
            lexicon.initialize( iss );
        }

        return standardize_addr( address_in, grammar, lexicon, locale_in, filter_in, err_msg );

    }
    catch ( std::runtime_error &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( std::exception &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( ... ) {
        *err_msg = strdup( "Caught unknown expection!" );
        return NULL;
    }
}


STDADDR *standardize_addr( char *address_in, Grammar & grammar, Lexicon & lexicon, char *locale_in, char *filter_in, char **err_msg)
{
    try {
        // Normalize and UPPERCASE the input string
        UErrorCode errorCode;
        std::string nstr = Utils::normalizeUTF8( std::string(address_in), errorCode );
        std::string Ustr = Utils::upperCaseUTF8( nstr, locale_in );

        Tokenizer tokenizer( lexicon );
        tokenizer.filter( InClass::asType( filter_in ) );

        std::vector<std::vector<Token> > phrases;
        phrases.push_back( tokenizer.getTokens( Ustr ) );

        Search search( grammar );

        float bestCost = -1.0;
        auto best = search.searchAndReclassBest( phrases, bestCost );

        if ( bestCost >= 0.0 ) {

            // get the appropriate standard terms and
            // collect then by their outclass
            // return them in STDADDR
            std::vector<std::string> v_stdaddr(16, "");
            for (auto &token : best) {
                lexicon.standardize( token );
                switch (token.outclass()) {
                    case OutClass::BLDNG:
                        v_stdaddr[0] += token.stdtext() + " ";
                        break;
                    case OutClass::HOUSE:
                        v_stdaddr[1] += token.stdtext() + " ";
                        break;
                    case OutClass::PREDIR:
                        v_stdaddr[2] += token.stdtext() + " ";
                        break;
                    case OutClass::QUALIF:
                        v_stdaddr[3] += token.stdtext() + " ";
                        break;
                    case OutClass::PRETYP:
                        v_stdaddr[4] += token.stdtext() + " ";
                        break;
                    case OutClass::STREET:
                        v_stdaddr[5] += token.stdtext() + " ";
                        break;
                    case OutClass::SUFTYP:
                        v_stdaddr[6] += token.stdtext() + " ";
                        break;
                    case OutClass::SUFDIR:
                        v_stdaddr[7] += token.stdtext() + " ";
                        break;
                    case OutClass::RR:
                        v_stdaddr[8] += token.stdtext() + " ";
                        break;
                    case OutClass::STOP:
                    case OutClass::EXTRA:
                    case OutClass::BADTOKEN:
                        v_stdaddr[9] += token.stdtext() + " ";
                        break;
                    case OutClass::CITY:
                        v_stdaddr[10] += token.stdtext() + " ";
                        break;
                    case OutClass::PROV:
                        v_stdaddr[11] += token.stdtext() + " ";
                        break;
                    case OutClass::NATION:
                        v_stdaddr[12] += token.stdtext() + " ";
                        break;
                    case OutClass::POSTAL:
                        v_stdaddr[13] += token.stdtext() + " ";
                        break;
                    case OutClass::BOXH:
                    case OutClass::BOXT:
                        v_stdaddr[14] += token.stdtext() + " ";
                        break;
                    case OutClass::UNITH:
                    case OutClass::UNITT:
                        v_stdaddr[15] += token.stdtext() + " ";
                        break;
                    case OutClass::IGNORE:
                        // do nothing with this
                        break;
                }
            }

            // allocate memory and fill up the STDADDR structure
            // using C-style allocation because it will get
            // freed on the C side of things.
            STDADDR *stdaddr    = (STDADDR*) calloc( sizeof(STDADDR), 1 );
            if ( ! stdaddr ) {
                *err_msg = strdup( "Out of memory!" );
                return NULL;
            }
            // assign strings with content and trim off the trailing blank
            if ( v_stdaddr[0].size() > 0 ) {
                v_stdaddr[0].resize(v_stdaddr[0].size()-1);
                stdaddr->building   = strdup( v_stdaddr[0].c_str() );
            }
            if ( v_stdaddr[1].size() > 0 ) {
                 v_stdaddr[1].resize(v_stdaddr[1].size()-1);
                stdaddr->house_num  = strdup( v_stdaddr[1].c_str() );
            }
            if ( v_stdaddr[2].size() > 0 ) {
                v_stdaddr[2].resize(v_stdaddr[2].size()-1);
                stdaddr->predir     = strdup( v_stdaddr[2].c_str() );
            }
            if ( v_stdaddr[3].size() > 0 ) {
                v_stdaddr[3].resize(v_stdaddr[3].size()-1);
                stdaddr->qual       = strdup( v_stdaddr[3].c_str() );
            }
            if ( v_stdaddr[4].size() > 0 ) {
                v_stdaddr[4].resize(v_stdaddr[4].size()-1);
                stdaddr->pretype    = strdup( v_stdaddr[4].c_str() );
            }
            if ( v_stdaddr[5].size() > 0 ) {
                v_stdaddr[5].resize(v_stdaddr[5].size()-1);
                stdaddr->name       = strdup( v_stdaddr[5].c_str() );
            }
            if ( v_stdaddr[6].size() > 0 ) {
                v_stdaddr[6].resize(v_stdaddr[6].size()-1);
                stdaddr->suftype    = strdup( v_stdaddr[6].c_str() );
            }
            if ( v_stdaddr[7].size() > 0 ) {
                v_stdaddr[7].resize(v_stdaddr[7].size()-1);
                stdaddr->sufdir     = strdup( v_stdaddr[7].c_str() );
            }
            if ( v_stdaddr[8].size() > 0 ) {
                v_stdaddr[8].resize(v_stdaddr[8].size()-1);
                stdaddr->ruralroute = strdup( v_stdaddr[8].c_str() );
            }
            if ( v_stdaddr[9].size() > 0 ) {
                v_stdaddr[9].resize(v_stdaddr[9].size()-1);
                stdaddr->extra      = strdup( v_stdaddr[9].c_str() );
            }
            if ( v_stdaddr[10].size() > 0 ) {
                v_stdaddr[10].resize(v_stdaddr[10].size()-1);
                stdaddr->city       = strdup( v_stdaddr[10].c_str() );
            }
            if ( v_stdaddr[11].size() > 0 ) {
                v_stdaddr[11].resize(v_stdaddr[11].size()-1);
                stdaddr->prov       = strdup( v_stdaddr[11].c_str() );
            }
            if ( v_stdaddr[12].size() > 0 ) {
                v_stdaddr[12].resize(v_stdaddr[12].size()-1);
                stdaddr->country    = strdup( v_stdaddr[12].c_str() );
            }
            if ( v_stdaddr[13].size() > 0 ) {
                v_stdaddr[13].resize(v_stdaddr[13].size()-1);
                stdaddr->postcode   = strdup( v_stdaddr[13].c_str() );
            }
            if ( v_stdaddr[14].size() > 0 ) {
                v_stdaddr[14].resize(v_stdaddr[14].size()-1);
                stdaddr->box        = strdup( v_stdaddr[14].c_str() );
            }
            if ( v_stdaddr[15].size() > 0 ) {
                v_stdaddr[15].resize(v_stdaddr[15].size()-1);
                stdaddr->unit       = strdup( v_stdaddr[15].c_str() );
            }
            *err_msg = (char *)0;
            return stdaddr;
        }
        // we failed to match
        *err_msg = (char *)0;
        return NULL;
    }
    catch ( std::runtime_error &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( std::exception &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( ... ) {
        *err_msg = strdup( "Caught unknown expection!" );
        return NULL;
    }

}

void *getGrammarPtr( char *grammar_in, char **err_msg )
{
    try {
        std::string s( grammar_in );
        std::istringstream iss( s );
        Grammar* grammar = new Grammar( iss );
        return static_cast<void*>(grammar);
    }
    catch ( std::runtime_error &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( std::exception &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( ... ) {
        *err_msg = strdup( "Caught unknown expection trying to load the Grammar!" );
        return NULL;
    }
}

void freeGrammarPtr( void *ptr )
{
    try {
        delete static_cast<Grammar*>( ptr );
    }
    catch ( ... ) {
        // NOP
    }
}

void *getLexiconPtr( char *lexicon_in, char **err_msg )
{
    try {
        std::string s( lexicon_in );
        std::istringstream iss( s );
        Lexicon* lexicon = new Lexicon;
        try {
            boost::archive::text_iarchive ia(iss);
            ia >> *lexicon;
        }
        catch (std::runtime_error& e) {
            if ( e.what() == std::string("Re-compile-lexicon"))
                throw;
            iss.clear();
            iss.str( s );
            lexicon->name( "query-lex" );
            lexicon->initialize( iss );
        }
        catch ( ... ) {
            iss.clear();
            iss.str( s );
            lexicon->name( "query-lex" );
            lexicon->initialize( iss );
        }
        return static_cast<void*>(lexicon);
    }
    catch ( std::runtime_error &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( std::exception &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( ... ) {
        *err_msg = strdup( "Caught unknown expection trying to load the Lexicon!" );
        return NULL;
    }
}

void freeLexiconPtr( void *ptr )
{
    try {
        delete static_cast<Lexicon*>(ptr);
    }
    catch ( ... ) {
        // NOP
    }
}

char *getGrammarMd5( void *ptr )
{
    try {
        return strdup( static_cast<Grammar*>(ptr)->getMd5() );
    }
    catch ( ... ) {
        return NULL;
    }
}

char *getLexiconMd5( void *ptr )
{
    try {
        return strdup( static_cast<Lexicon*>(ptr)->getMd5() );
    }
    catch ( ... ) {
        return NULL;
    }
}

char *getMd5( char *text )
{
    try {
        std::string md5str = md5( text );
        return strdup( md5str.c_str() );
    }
    catch ( ... ) {
        return NULL;
    }
}

char * serialize_lexicon( char *lexicon_in, char **err_msg )
{
    try {

        // put the lexicon into a string stream
        std::string s( lexicon_in );
        std::istringstream iss( s );

        // create the lexicon object
        Lexicon lex( "query-lex", iss );

        // populate the regex strings in the Lexicon
        lex.regex();
        lex.regexPrefixAtt();
        lex.regexSuffixAtt();

        // serialize the lexicon object into a string stream
        std::ostringstream ofs;
        boost::archive::text_oarchive oa(ofs);
        oa << lex;

        // get the serialized lexicon text and return it
        std::string clex = ofs.str();
        if (clex.size() == 0) {
            *err_msg = strdup( "Compiled lexicon should not be size=0!" );
            return NULL;
        }

        *err_msg = (char *)0;
        return strdup(clex.c_str());

    }
    catch ( std::runtime_error &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( std::exception &e ) {
        *err_msg = strdup( e.what() );
        return NULL;
    }
    catch ( ... ) {
        *err_msg = strdup( "Caught unknown expection!" );
        return NULL;
    }
    
}
