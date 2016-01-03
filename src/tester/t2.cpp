// char_sep_example_2.cpp
#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    std::string str = ";;Hello|world||-foo--bar;yow;baz|";
    typedef boost::tokenizer<boost::char_separator<char> > 
        tokenizer;
    boost::char_separator<char> sep("-;", "|", boost::keep_empty_tokens);
    tokenizer tokens(str, sep);
    for (tokenizer::iterator tok_iter = tokens.begin();
         tok_iter != tokens.end(); ++tok_iter)
      std::cout << "<" << *tok_iter << "> ";
    std::cout << "\n";
    return EXIT_SUCCESS;
}
