// char_sep_example_2.cpp
#include <iostream>
#include <map>
#include <boost/regex.hpp>
#include <string>

typedef std::map<std::string, int, std::less<std::string> > map_type;

void tokenize(map_type &m, const std::string& str) {

    boost::regex expression(
        "^(\\d+/\\d+|\\d+|\\<[[:alpha:]]+\\>|\\w+)([-\\s\\|[:punct:]]+|$)",
        boost::regex::icase
    );;

    std::string::const_iterator start, end;
    start = str.begin();
    end = str.end();

    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;

    while(boost::regex_search(start, end, what, expression, flags)) {

        std::cout << "Searcing string: '"
                  << std::string(start, end)
                  << "'\n";
        std::cout << "0: '" << std::string(what[0].first, what[0].second)
            << "', " << what[0].first-str.begin()
            << ", "  << what[0].second-str.begin()
            << "\n";
        std::cout << "1: '" << std::string(what[1].first, what[1].second)
            << "', " << what[1].first-str.begin()
            << ", "  << what[1].second-str.begin()
            << "\n";
        std::cout << "2: '" << std::string(what[2].first, what[2].second)
            << "', " << what[2].first-str.begin()
            << ", "  << what[2].second-str.begin()
            << "\n";

        // update search position
        start = what[0].second;
        std::cout << "Remaining string: '"
                  << std::string(start, end)
                  << "'\n";
        // update flags
//        flags |= boost::match_prev_avail;
        flags |= boost::match_not_bob;
    }
}

int main() {

    map_type m;

    //std::string str = "1/5;;Hello|world||-foo--bar;yow;baz|";
    std::string str = "Hello,world,,-foo--bar;yow;baz";

    tokenize(m, str);

    return EXIT_SUCCESS;
}
