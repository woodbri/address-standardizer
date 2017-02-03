# address-standardizer
An address parser and standardizer in C++

The goal is to be able to take an address string and parse it into tokens
classify and standardize the tokens using an appropriate lexicon, then map the
tokens to specific output fields based on rules.

The purpose of this code is to be used as part of a geocoding or record
matching system and is not useful for standardizing addresses for postal
correctness or delivery point solutions.

## Dependencies

* boost/regex.hpp
* boost/regex/icu.hpp
* boost/lexical\_cast.hpp
* boost/algorithm/string.hpp
* boost/algorithm/string/join.hpp
* boost/algorithm/string/split.hpp
* boost/algorithm/string/classification.hpp
* boost/serialization/\*
* unicode/\*  (from icu packages)
* PostgreSQL server development for 9.2, 9.3, 9.4, 9.5, and/or 9.6

### For Ubuntu/Debian

```
sudo apt-get install libicu-dev libicu52 libboost1.55-dev libboost-serialization1.55-dev libboost-serialization1.55.0 libboost-regex1.55-dev libboost-regex1.55.0
```

## Build

See file src/README.address\_standardizer

## Funding Needed

There are a lot of pieces to this project and funding will help it along.
There is a lot of code to write, test and document. Beyond that we need to
develop lexicons and rules addressing standards for every country that one
might want to support.

Please contact me if you are interested in this project and/or you can
provide some amount of funding.  woodbri (at) imaptools (dot) com.

