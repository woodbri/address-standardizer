/*HEADER*
*HEADER*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/regex.hpp>

#include "grammar.h"

Grammar::Grammar( const std::string &file ) {

    std::string line;
    std::ifstream is (file.c_str(), std::ifstream::in|std::ifstream::binary);
    if (not is.is_open()) {
        std::cerr << "ERROR: Failed to open file: '" << file << "'\n";
        //throw 2000
        return;
    }

    boost::regex re_comment("^\\s*#|^\\s*$");
    boost::regex re_section("^\\s*\\[(.+)\\]\\s*$");
    boost::regex re_ismeta("^(?:\\s*@([\\w_])+)+\\s*$");

    std::string gotSection;
    std::vector<Rule> rules;

    boost::smatch what;
    int line_cnt = 0;
    while (not is.eof()) {
        ++line_cnt;
        std::getline( is, line );
        //std::cout << "\t" << line_cnt << ": " << line << "\n";

        // skip over comments and blank lines
        if ( boost::regex_match(line, re_comment, boost::match_default) ) 
            continue;
        // handle a section header
        else if ( boost::regex_match(line, what, re_section, boost::match_extra) ) {
            // if we already have section data, then save 
            if (gotSection.length() > 0) {
                rules_[gotSection] = rules;
                gotSection.clear();
                rules.clear();
            }
            gotSection = what[0];
        }
        // load data into section
        else {
            if (gotSection.length() == 0) {
                std::cerr << "ERROR: trying to load grammar section data outside of a section!\n";
                std::cerr << line_cnt << ": " << line << "\n";
                exit(1);
            }

            Rule rule;

            // check if this is a meta rule
            if ( line.find('@') != std::string::npos ) {
                if (boost::regex_match(line, what, re_ismeta, boost::match_default) ) {
                    // extract names
                    for (auto i=0; i<what.size(); ++i)
                        rule.meta.push_back(what[i]);
                }
                else {
                    // ERROR all names must match /@([\w_]+)/
                    std::cerr << "ERROR: all names must match pattern /@([\\w_]+)/ on meta rule lines!\n";
                    std::cerr << line_cnt << ": " << line << "\n";
                    exit(1);
                }
            }
            // otherwise it is a standard rule line
            else {

            }
        }
    }
    // save the last section with read in
    if (gotSection.length() > 0) {
        rules_[gotSection] = rules;
        gotSection.clear();
        rules.clear();
    }

    is.close();

    grammarChecker();
}


std::ostream &operator<<(std::ostream &ss, const Grammar &g) {

}
