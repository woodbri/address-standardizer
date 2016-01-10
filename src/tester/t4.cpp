

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

    std::string str("");
    for(int i=1; i<ac; i++)
        str += av[i];

    std::cout << "str: '" << str << "'\n";

    std::deque<Token> tok;

    Lexicon lex;

    Tokenizer tokenizer( lex );
    tokenizer.addFilter( InClass::PUNCT );
    tok = tokenizer.getTokens( str );

    std::cout << "tok.size() = " << tok.size() << "\n";

    for (auto it = tok.begin(); it != tok.end(); it++) 
        std::cout << *it << "\n";

    return EXIT_SUCCESS;
}
