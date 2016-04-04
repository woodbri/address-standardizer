# address-standardizer
An address parser and standardizer in C++

The goal is to be able to take an address string and parse it into tokens
classify and standardize the tokens using an appropriate lexicon, then map the
tokens to specific output fields based on rules.

The purpose of this code is to be used as part of a geocoding or record
matching system and is not useful for standardizing addresses for postal
correctness or delivery point solutions.

## Build

```
cd src
make clean; make

cd tester
make test
```

Eventually this will get wrapped into postgresql stored procedures.

## Funding Needed

There are a lot of pieces to this project and funding will help it along.
There is a lot of code to write, test and document. Beyond that we need to
develop lexicons and rules addressing standards for every country that one
might want to support.

Please contact me if you are interested in this project and/or you can
provide some amount of funding.  woodbri (at) imaptools (dot) com.

