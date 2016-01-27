

#include "lexicon.h"

#include <iostream>
#include <string>
#include <vector>

int main(int ac, char* av[]) {

    if (ac != 2) {
        std::cerr << "Usage: t1 lex.txt\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    std::cout << "file: '" << file << "'\n";

    Lexicon lex("test-lex", file);

    std::cout << lex << "\n\n";

    return EXIT_SUCCESS;
}
