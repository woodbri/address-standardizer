// test the grammar reader and check()

#include <iostream>
#include <string>

#include "grammar.h"



int main(int ac, char* av[])
{
    if (ac < 2) {
        std::cerr << "Usage: t3 test.grammar\n";
        return EXIT_FAILURE;
    }

    std::string file = av[1];

    Grammar g(file);

    std::cout << g << "\n";

    return EXIT_SUCCESS;
}
