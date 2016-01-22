

#include "../inclass.h"
#include "../lexentry.h"
#include "../lexicon.h"
#include "../outclass.h"
#include "../token.h"
#include "../tokenizer.h"
#include "../utils.h"

#include <iostream>
#include <string>
#include <deque>

int main(int ac, char* av[]) {

    if (ac < 3) {
        std::cerr << "Usage: t4 lex.txt 'address to parse'\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];
    std::string str = av[2];

//    std::cout << "Lexicon: '" << file << "'\n";
    std::cout << "Address: '" << str << "'\n";

    // Normalize and UPPERCASE the input string
    UErrorCode errorCode;
    std::string nstr = Utils::normalizeUTF8( str, errorCode );
    std::string Ustr = Utils::upperCaseUTF8( nstr, "en" );

    std::cout << "Normalized: '" << nstr << "'\n";
    std::cout << "UpperCase: '" << Ustr << "'\n";

    std::deque<Token> tokens;

    Lexicon lex("test-lex", file);
//    std::cout << lex << "\n";
//    std::cout << "Lexicon regex: '" << lex.regex() <<"'\n\n";
//    std::cout << "Lexicon attachedRegex: '" << lex.attachedRegex() <<"'\n\n";

    Tokenizer tokenizer( lex );
    tokenizer.addFilter( InClass::PUNCT );
    tokens = tokenizer.getTokens( Ustr );

//    std::cout << "tokens.size() = " << tokens.size() << "\n";

    for (const auto tok : tokens)
        std::cout << tok << "\n";

    return EXIT_SUCCESS;
}
