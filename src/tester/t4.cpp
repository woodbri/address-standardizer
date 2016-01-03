// char_sep_example_2.cpp
#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    //std::string str = "ABI�A AUZOA, Z/G  SUKARRIETA BIZKAIA  48395";
    //std::string str = "Albert-Schweitzer-Weg, Süd II";
    //std::string str = "Ahornstra▒~_e, Goldenst. Nord II";
    std::string str = "Alte Bundestra▒~_e (Harpendorf)";
    typedef boost::tokenizer<boost::char_separator<char> > 
        tokenizer;
    //boost::char_separator<char> sep(" ", ",-", boost::keep_empty_tokens);
    boost::char_separator<char> sep(" \t", "!@#$%^&*()-+={}|[]:\";'<>,.?/");
    tokenizer tokens(str, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
         tok_iter != tokens.end(); ++tok_iter)
      std::cout << "<" << *tok_iter << "> ";
    std::cout << "\n";
    return EXIT_SUCCESS;
}
