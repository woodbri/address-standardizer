# Address Standardizer

## What Is It?

The Address Standardizer is a set of C++ classes designed to deal with the
problem of standardizing street addresses for the purpose of geocoding.
Geocoding is the process of matching an address to a reference set of streets.
Or in more general terms it can be used for record linkage between data dataset
be matching a record in one data set to record(s) in another data set.

Some of the key design features are:

* Support for UTF8 data to support countries around the world
* Human readable Lexicon and Grammar file
* Support for splitting attached street types like in German
* Wrappers to make it callable as a PostgreSQL stored procedure
* C++ OO Class structure

## Why You Might Care?

If you need to build a geocoder or a record linking application then this is a
key component that can help in that task. Additionally, if you need to change
the underlying data that is being standardized for example by adding a new
country to the standardizer or adding an new reference set, then this document
will help explain the how it works and what you can do to make those changes.

## How Does It Work?

There are a few main modules in the code: Lexicon, Tokenizer, Grammar, Search
classes that work together to take and address, parse it into tokens, then
search the grammar for a match to tokens. The end result is that based on the
matching we can reclassify the input tokens into a standardized stream of
output tokens.

### Example Problem

    Address: 11 Radcliffe Rd North Chelmsford MA 01863 USA
    Input Tokens:  NUMBER WORD TYPE DIRECT WORD PROV NUMBER NATION
    Output Tokens: HOUSE STREET SUFTYP CITY CITY PROV POSTAL NATION

In the above example the address is Tokenized using the Lexicon to help classify
the input tokens. Next we search the Grammar looking for a match to the input
tokens. If we match the grammar, the input tokens are reclassified by the
Grammar rules to the output tokens.

This is a very simple example to get the idea across. More detailed
explanations follow.

### Lexicon Class

The Lexicon class is responsible for:

* loading data into the lexicon
* classifying tokens
* working with the Tokenizer

Currently data is loaded from text files or PostgreSQL tables when wrapped into
a stored procedure. The file format for the data is explained below. The
lexicon can contain words or phrases and each has a list of possible
classifications. For example the word **NORTH** could be a direction indicator
when used in "11 North Main St" but is just a part of a city name in "North
Chelmsford". In the former we might classify **NORTH** as **DIRECT** and in the
later just as **WORD**. You will see this in the file format example below. The
Lexicon also support defining abbreviations and/or common misspellings that you
might want to be able to recognize and classify.

### Tokenizer Class

### Grammar Class

### Search Class

### Lexicon File Format

### Grammar File Format

### Existing Lexicons and Grammars
