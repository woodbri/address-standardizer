

#include "../inclass.h"
#include "../lexentry.h"
#include "../lexicon.h"
#include "../outclass.h"
#include "../token.h"
#include "../tokenizer.h"
#include "../utils.h"
#include "../grammar.h"
#include "../search.h"

#include <iostream>
#include <string>
#include <vector>

int main(int ac, char* av[]) {

    if (ac < 4) {
        std::cerr << "Usage: t2 lex.txt grammar.txt 'address to parse'\n";
        return EXIT_FAILURE;
    }

    std::string lfile = av[1];
    std::string gfile = av[2];
    std::string str = av[3];

//    std::cout << "Lexicon: '" << file << "'\n";
    std::cout << "Address: '" << str << "'\n";

    // Normalize and UPPERCASE the input string
    UErrorCode errorCode;
    std::string nstr = Utils::normalizeUTF8( str, errorCode );
    std::string Ustr = Utils::upperCaseUTF8( nstr, "en" );

    std::cout << "Normalized: '" << nstr << "'\n";
    std::cout << "UpperCase: '" << Ustr << "'\n";

    std::vector<Token> tokens;

    Lexicon lex("test-lex", lfile);
//    std::cout << lex << "\n";
//    std::cout << "Lexicon regex: '" << lex.regex() <<"'\n\n";
//    std::cout << "Lexicon attachedRegex: '" << lex.attachedRegex() <<"'\n\n";

    Tokenizer tokenizer( lex );
    tokenizer.addFilter( InClass::PUNCT );
    tokenizer.addFilter( InClass::DASH );
    tokens = tokenizer.getTokens( Ustr );

//    std::cout << "tokens.size() = " << tokens.size() << "\n";

    for (const auto tok : tokens)
        std::cout << tok << "\n";

    std::vector< std::vector<InClass::Type> > list = Token::enumerate( tokens );

    std::cout << "Enumerated token list size: " << list.size() << "\n";
    for (const auto &e : list) {
        for (const auto &t : e )
            std::cout << InClass::asString( t ) << "  ";
        std::cout << "\n";
    }
    std::cout << "\n";

    Grammar G( gfile );

    Search S( G );

    for (const auto &e : list) {
        if ( S.search( e ) ) {
            std::cout << "#### SUCCESS!\n";
            std::vector<Rule> best = S.bestResult();
            for (const auto &e : best)
                std::cout << e << "\n";
            std::cout << "--------------------------------------------\n";
        }
        else {
            std::cout << "#### FAILED!\n";
        }
    }

    return EXIT_SUCCESS;
}
