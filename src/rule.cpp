/*HEADER*
*HEADER*/

#include <sstream>
#include <boost/regex.hpp>

#include "rule.h"


Rule::Rule( const std::string &line, const bool isMeta ) : score_(0.0) {
    std::string token;
    std::stringstream buffer(line);

    // read meta rule here
    if (isMeta) {
        boost::regex re_ismeta("@([\\w_]+)");
        boost::smatch what;

        std::string::const_iterator start, end;
        start = line.begin();
        end   = line.end();

        while ( boost::regex_search( start, end, what, re_ismeta ) ) {
            if (what[1].first < what[1].second)
                pushMeta( std::string( what[1].first, what[1].second ) );
            start = what[0].second;
        }
    }
    // else read normal rule here
    else {
        int state = 0;
        while (not buffer.eof()) {
            buffer >> token;

            if (token == "->" ) {
                ++state;
                continue;
            }

            switch (state) {
                case 0:     // reading inClass tokens
                    pushIn( InClass::asOneType( token ) );
                    break;
                case 1:     // reading outClass tokens
                    pushOut( OutClass::asType( token ) );
                    break;
                case 2:     // reading score
                    score( atof( token.c_str() ) );
                    break;
            }
        }
    }

    if (not isValid()) {
        std::cerr << "ERROR: Invalid Rule defined for: '" << line << "'\n";
        //clear();
    }
}


bool Rule::isValid() const {
    if ( isMeta() and (
         inClass_.size() != 0 or
         outClass_.size() != 0 or
         score_ != 0.0 ))
        return false;
    if ( not isMeta() and inClass_.size() != outClass_.size() )
        return false;

    return true;
}


// algorithms

Rule Rule::concat( const Rule &r ) const {
    Rule conc;

    if ( isMeta() != r.isMeta() ) 
        return conc;

    if ( isMeta() ) {
        conc.meta( meta_ );
        std::vector<std::string> meta = r.meta();
        for (const auto &s : meta )
            conc.pushMeta( s );
    }
    else {

        // copy this to conc
        conc.in( inClass_ );
        conc.out( outClass_ );

        // append r to conc
        std::vector<InClass::Type> in = r.in();
        for ( const auto &e : in )
            conc.pushIn( e );

        std::vector<OutClass::Type> out = r.out();
        for ( const auto &e : out )
            conc.pushOut( e );

        conc.score( (score_ + r.score()) / 2.0 );
    }

    return conc;
}


// setters

void Rule::clear() {
    meta_.clear();
    inClass_.clear();
    outClass_.clear();
    score_ = 0.0;
}


void Rule::pushMeta( const std::string &s ) {
    meta_.push_back(s);
    inClass_.clear();
    outClass_.clear();
    score_ = 0.0;
}


void Rule::pushIn( const InClass::Type t ) {
    inClass_.push_back(t);
    meta_.clear();
}


void Rule::pushOut( const OutClass::Type t ) {
    outClass_.push_back(t);
    meta_.clear();
}


void Rule::pushInOut(const InClass::Type in, const OutClass::Type out) {
    inClass_.push_back(in);
    outClass_.push_back(out);
}

void Rule::meta( const std::vector<std::string> &meta ) {
    meta_ = meta;
    inClass_.clear();
    outClass_.clear();
    score_ = 0.0;
}


void Rule::in( const std::vector<InClass::Type> &in ) {
    inClass_ = in;
    meta_.clear();
}


void Rule::out( const std::vector<OutClass::Type>  &out ) {
    outClass_ = out;
    meta_.clear();
}


void Rule::score( const float s ) {
    score_ = s;
    meta_.clear();
}


std::ostream &operator<<(std::ostream &ss, const Rule &r) {
    std::string word;

    if ( r.isMeta() ) {
        std::vector<std::string> meta = r.meta();
        for ( const auto &e : meta )
            ss << "@" << e;
        ss << "\n";
    }
    else {
        std::vector<InClass::Type> inClass = r.in();
        for ( const auto &e : inClass )
            ss << InClass::asString(e) << " ";
        ss << "-> ";
        std::vector<OutClass::Type> outClass = r.out();
        for ( const auto &e : outClass )
            ss << OutClass::asString(e) << " ";
        ss << "-> " << r.score();
    }

    return ss;
}


