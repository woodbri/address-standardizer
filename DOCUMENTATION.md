# Address Standardizer

Author: Stephen Woodbridge

## What Is It?

The Address Standardizer is a set of C++ classes designed to deal with the
problem of standardizing street addresses for the purpose of geocoding.
Geocoding is the process of matching an address to a reference set of streets.
In more general terms, this library can be used for record linkage between data
dataset be matching a record in one data set to record(s) in another data set.

Some of the key design features are:

* Support for UTF8 data to support multiple languages
* Human readable Lexicon and Grammar file
* Support for splitting attached street types like in German
* Wrappers to make it callable as a PostgreSQL stored procedure
* C++ OO Class structure

It is **not** intended to be a CASS Address correction or standardizing tool.
If you do not know what this means then read on.

## Why You Might Care?

If you need to build a geocoder or a record linking application then this is a
key component that can help in that task. Additionally, if you need to change
the underlying data that is being matched for example by adding a new
country to the standardizer or adding an new reference set, then this document
will help explain the how it works and what you can do to make those changes.

## How Does It Work?

It parses a string into tokens and assigns an input classification to each one
using a lexicon. It then matches the stream of input tokens against a grammar
to decide how to best map this token stream to a set of output classification
based on the grammar so we can understand the structure of the text.

There are a few main modules in the code the work together to accomplish this:
Lexicon, Tokenizer, Grammar, Search classes that work together to take and
address, parse it into tokens, then search the grammar for a match to tokens.
The end result is that based on the matching we can reclassify the input tokens
into a standardized stream of output tokens.

### Example Problem

Here is an example address that might get parsed to the following input tokens.
Then after matching the input tokens to a grammar, we can identify each token
by its output classification to understand how to map the words in the address
to components of an address.

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

The Tokenizer class is responsible for parsing the string into tokens using the
lexicon to recognize words and phrases, to split attached words and work with
the lexicon to classify the input tokens. The input is a string and the output
is a vector of tokens.

There is a SQL function ``as_parse(address, lexicon, locale, filter)`` that can
help you understand how the tokenizer is parsing an address and breaking it
into tokens. This is important because it is this string of tokens that you
want your grammar to match against. If you find an address that fails to
standardize, then run it through ``as_parse`` and look at the tokens and make
sure you have rules in the grammar that can match this string of tokens.
Another option is that if you do not like the string of tokens, maybe you can
add some entries in the lexicon to recognize some of the words and classify
them explicitly to generate a better string of tokens.

Here are some examples:

```
as_test=# select * from as_parse(
    '1/1 radcliff rd, north chelmsford, ma 01863 usa',
    (select clexicon from as_config where countrycode='us'),
    'en_US',
    (select filter from as_config where countrycode='us')
);
 seq |    word    |       inclass       | attached
-----+------------+---------------------+----------
   1 | 1/1        | FRACT               |
   2 | RADCLIFF   | WORD                |
   3 | RD         | TYPE                |
   4 | NORTH      | DIRECT              |
   5 | CHELMSFORD | WORD                |
   6 | MA         | WORD,TYPE,ROAD,PROV |
   7 | 01863      | NUMBER,QUINT        |
   8 | USA        | NATION              |
(8 rows)

as_test=# select * from as_parse(
    '1/a radcliff rd, north chelmsford, ma 01863 usa',
    (select clexicon from as_config where countrycode='us'),
    'en_US',
    (select filter from as_config where countrycode='us')
);
 seq |    word    |        inclass         | attached
-----+------------+------------------------+----------
   1 | 1          | NUMBER                 |
   2 | /          | SLASH                  |
   3 | A          | QUALIF,STOPWORD,SINGLE |
   4 | RADCLIFF   | WORD                   |
   5 | RD         | TYPE                   |
   6 | NORTH      | DIRECT                 |
   7 | CHELMSFORD | WORD                   |
   8 | MA         | WORD,TYPE,ROAD,PROV    |
   9 | 01863      | NUMBER,QUINT           |
  10 | USA        | NATION                 |
(10 rows)

```

In this example, "1/1" is being recognized as a ``FRACT`` token. And "1/a" is parsed as ``NUMBER SLASH QUALIF,STOPWORD,SINGLE``. But if we put
a space to the right or left of the slash we get:

```
as_test=# select * from as_parse(
    '1 /1 radcliff rd, north chelmsford, ma 01863 usa',
    (select clexicon from as_config where countrycode='us'),
    'en_US',
    (select filter from as_config where countrycode='us')
);
 seq |    word    |       inclass       | attached
-----+------------+---------------------+----------
   1 | 1          | NUMBER              |
   2 | /          | SLASH               |
   3 | 1          | NUMBER              |
   4 | RADCLIFF   | WORD                |
   5 | RD         | TYPE                |
   6 | NORTH      | DIRECT              |
   7 | CHELMSFORD | WORD                |
   8 | MA         | WORD,TYPE,ROAD,PROV |
   9 | 01863      | NUMBER,QUINT        |
  10 | USA        | NATION              |
(10 rows)
```

If we remove the filter by setting it to '', you will also see all the
separator tokens between the regular tokens:

```
as_test=# select * from as_parse(
    '1 /1 radcliff rd, north chelmsford, ma 01863 usa',
    (select clexicon from as_config where countrycode='us'),
    'en_US',
    ''
);
 seq |    word    |       inclass       | attached
-----+------------+---------------------+----------
   1 | 1          | NUMBER              |
   2 | /          | SLASH               |
   3 | 1          | NUMBER              |
   4 |            | SPACE               |
   5 | RADCLIFF   | WORD                |
   6 |            | SPACE               |
   7 | RD         | TYPE                |
   8 | ,          | PUNCT               |
   9 | NORTH      | DIRECT              |
  10 |            | SPACE               |
  11 | CHELMSFORD | WORD                |
  12 | ,          | PUNCT               |
  13 | MA         | WORD,TYPE,ROAD,PROV |
  14 |            | SPACE               |
  15 | 01863      | NUMBER,QUINT        |
  16 |            | SPACE               |
  17 | USA        | NATION              |
(17 rows)
```

In this last case, you might notice that the space in "1 /1" does not show in the output. This is because a string of multiple separators is recognized as a single separator and extra spaces are trimmed off leaving just the "/".

### Grammar Class

The Grammar class is responsible for loading a grammar and building the
internal structures to hold it. A grammar is stored as a collection of named Rules.

### Search Class

The Search class extends a grammar and provides the methods for matching a stream of input tokens to a grammar. It uses a recursive search algorithm to match the input tokens to the grammar.

### Lexicon File Format

These files are required to be in UTF8 data.

The following is a sample of lines from the **lex-usa.txt** file

    LEXICON:    lex-usa.txt ENG en_US
    LEXENTRY:   #   APT UNITH   DETACH
    LEXENTRY:   9   9   NUMBER  DETACH
    LEXENTRY:   90  90  NUMBER  DETACH
    LEXENTRY:   90TH    90TH    WORD,ORD    DETACH
    LEXENTRY:   9TH 9TH WORD,ORD    DETACH
    LEXENTRY:   AK  Alaska  PROV    DETACH
    LEXENTRY:   AL  Alabama PROV    DETACH
    LEXENTRY:   ALLEE   ALY TYPE    DET_SUF
    LEXENTRY:   ALLEY   ALY TYPE    DET_SUF
    LEXENTRY:   ALLY    ALY TYPE    DET_SUF
    LEXENTRY:   ALY ALY TYPE    DET_SUF

The file is tab separated columns because you can have a phrase with spaces
within a field.

    LEXICON:	<filename>	<lang>	<locale>
    LEXENTRY:	<word>	<stdword>	<InClass::Type>	<InClass::AttachType>

where:

* &lt;filename> - not used, but handy for managing a large number of files
* &lt;lang> - not used, see **inclass.h** for a list of language codes
* &lt;locale> - the UTF8 locale for this data used to normalize and uppercase strings.
* &lt;word> - the word or phrase that represents this entry.
* &lt;stdword> - the standardized word or phrase for this entry.
* &lt;InClass::Type> - a comma separated list of InClass::Type(s) for this entry.
* &lt;InClass::AttachType> - a comma separated list of possible attachment types.

See **inclass.h** for the list of &lt;InClass::Type> and &lt;InClass::AttachType> values. **DETACH** is used represent no attachments. In lex-german.txt you will find a good example of attachments with this entry:

    LEXENTRY:   STRASSE STRASSE TYPE    ATT_SUF,DET_PRE,DET_SUF
    LEXENTRY:   STRAßE  STRASSE TYPE    ATT_SUF,DET_PRE,DET_SUF

These entries represent the German word for STREET and you can see from the attachment list that this entry can be:

* ATT_SUF - Attached at the suffix of a street name like: Mühlenstrasse
* DET_PRE - Prefix detached from the street name
* DET_SUF - Suffix detached from the street name like: Lange Strasse

Not included in the list above are:

* ATT_PRE - An attached prefix
* DETACH - No attachment (this is not listed inclass.h).

Lexicon files can be compiled and these compiled files load 10 times faster
than the original lexicon data files. In most cases, the code will accept
either form of the lexicon data. If you load the sample data files from the
sql it will create a compiled version of the lexicon in the ``clexicon``
column. If the format of the compiled lexicon changes, you will get an error
message indicating that you need to re-compile the lexicon.

### Grammar File Format

In general, the grammar files should be built to work with specific lexicons
and these need to be build based on a specific dataset and/or country. While
there may be a lot of commonality between data sets for a specific country,
each vendor will have their own idiosyncratic terms and phrases that need to be
recognized and classified.

The grammar is made up of a collection of rules. There are two types of rules:

1. meta rule that refers to other rules
2. terminal rules that refer to a partial stream of &lt;InClass::Type>

The system currently requires the root rule to be named "ADDRESS", beyond that you can name your rules using only letters, numbers and '_'. References to rules require the preceeding '@' character, like "@STREET_ADDRESS" below to refer to the "[STREET_ADDRESS]" rule. Comment line begin with a '#' character and blank lines are ignored.

The file syntax is:

    comment := '#' TEXT{0+} NEWLINE
    file := rule_group{1+} | comment{0+} | NEWLINE{0+}
    rule_group := meta_group | terminal_group
    identifier := ( LETTER | NUMBER | '_' ){1+}
    header := '[ADDRESS]'{1} | '[' identifier ']'
    meta_group := header meta_rules
    terminal_group := header terminal_rules
    meta_reference := '@' identifier
    meta_rules := ( meta_reference+ NEWLINE ){1+}
    rule := InClass::Type{n} '->' OutClass::Type{n} '->' FLOAT
    terminal_rules := ( rule NEWLINE ){1+}

The meta rules look like the follow:

    [ADDRESS]
    @STREET_ADDRESS @MACRO
    @PLACENAME @STREET_ADDRESS @MACRO

The "[ADDRESS]" is the rule name. The "@&lt;name>" is a reference to another rule.
This "[ADDRESS]" rule is saying an address is composed of matching the
"STREET_ADDRESS" rule followed by matching the "MACRO" rule. Or by matching the
"PLACENAME" rule followed by the "STREET_ADDRESS" rule followed by the "MACRO"
rule. So the grammar can be broken into convenient pieces and chained together
using the meta rules.

For example, "STREET_ADDRESS" might also refer to another meta rule like:

    [STREET_ADDRESS]
    @HOUSE_NUMBER @STREET_NAME
    @HOUSE_NUMBER @STREET_NAME @SECOND_UNIT
    @STREET_NAME @HOUSE_NUMBER
    @STREET_NAME @HOUSE_NUMBER @SECOND_UNIT

The "STREET_NAME" rule is a simple example of a terminal rule:

    [STREET_NAME]
    TYPE WORD   -> PRETYP STREET -> 0.3
    WORD TYPE   -> STREET SUFTYP -> 0.3

Terminal rules are constructed as:

    <InClass::Type> list '->' <OutClass::Type> list '->' <score>

the number of items in two lists **must** be the same. The &lt;score> can be
considered the probability for this rule. When a string is parsed into tokens,
any given token might have multiple classification. We enumerate all the
possible combinations of token classes and then match each to the grammar. We
might get multiple matches and the score allow us to select the best possible
match.

The purpose of these rules is to take a stream of input tokens and map them to
appropriate output tokens. The goal is to understand that a given token like
WORD in the input really belongs to the street name or maybe the city name,
etc.

So reviewing the original example:

    Address: 11 Radcliffe Rd North Chelmsford MA 01863 USA
    Input Tokens:  NUMBER WORD TYPE DIRECT WORD PROV NUMBER NATION
    Output Tokens: HOUSE STREET SUFTYP CITY CITY PROV POSTAL NATION

In this example, the first WORD token gets mapped to the STREET token and
the second WORD token get mapped to the CITY token. This allow us to match
"Radcliffe" against street names and Chelmsford against city names. 

### Existing Lexicons and Grammars

The project has an collection of lexicons for various countries in Europe,
United States and Canada. (See the **data/sample** directory). These lexicons
are only a starting place for users and will need to be adapted for your
specific needs.

Likewise there are starter grammars in the **data/sample/** directory.

Files ending in **.txt|.lex|.gmr** can be used with the file loader classes and files ending in **.sql** can be loaded as tables in the database and passed into the stored procedure interfaces in PostgreSQL.

## Performance Considerations

For each address that we need to standardize, we need both the lexicon and the
grammar objects. These are expensive to read and construct each time we need
them. We have implemented two features the can significantly improve
performance around this issue.

### Compile the Lexicon

It takes time to load a lexicon, especially if it is large, but if you compile
the lexicon using ``as_compile_lexicon`` and save it, then supplying the
compiled lexicon instead of the source lexicon saves about 66% of the time to
load the lexicon. If you load the sample lexicons and grammars, we create a
``clexicon`` column and compile the lexicon into that. Or you could do
something like:

```
update as_config set clexicon = as_compile_lexicon( lexicon );
```

which would compile or recompile all the lexicons in the ``as_config`` table.

### Query-Level Caching of Lexicon and Grammar objects

We implemented Query-Level Caching of Lexicon and Grammar objects to speed up
standardizing a whole table. You will not see any benefit on one-off queries
but if you construct your queries like the following, there is a significant
performance boost when standardizing a table of addresses.

For this example, assume that we have a table ``test_addresses`` with a column ``address`` and we want to standardize the text in ``address`` into a new table ``standardized_addresses`` that contains ``id`` from ``test_addresses``, the standardized fields followed by the ``test_addresses.address`` field.

```
-- before PostgreSQL 9.3 use a query like this
create table standardized_addresses as
with tmp as (select a.id, address, as_standardize(
                 address,
                 grammar,
                 clexicon,
                 'en_US',
                 filter) as std
             from as_config cfg, test_addresses a
            where cfg.countrycode='us')
select id, (std).*, address
  from tmp
 order by id;

-- PostgreSQL 9.3+ use a query like this
select a.id, std.*, address
  from test_addresses as a,
       as_config cfg,
       LATERAL as_standardize(
            address,
            grammar,
            clexicon,
            'en_US',
            filter
        ) as std
 where cfg.countrycode='us'
 order by a.id;

```

You can get a little more creative if your ``test_addresses`` table looks like
this and you are working with addresses from multiple countries. Given that the
``as_config`` looks like the following.

```
CREATE TABLE as_config
(
  id serial NOT NULL PRIMARY KEY,
  countrycode character(2),
  countryname text,
  dataset text,
  lexicon text,
  clexicon text,
  grammar text,
  filter text
);
```

You might create your ``test_addresses`` table like:

```
CREATE TABLE test_addresses
(
    id serial NOT NULL PRIMARY KEY,
    address text,
    locale text,
    countrycode character(2)
);
```

Now ``test_addresses.countrycode`` can join on ``as_config.countrycode`` and in
this case we can now use multiple lexicons and grammars where each is
appropriate to address record getting standardized, using queries like:

```
-- before PostgreSQL 9.3 use a query like this
create table standardized_addresses as
with tmp as (select a.id, address, as_standardize(
                 address,
                 grammar,
                 clexicon,
                 a.locale,
                 filter) as std
             from as_config cfg, test_addresses a
            where a.countrycode=cfg.countrycode
            order by a.countrycode)
select id, address, (std).* from tmp;

-- PostgreSQL 9.3+ use a query like this
select a.id, address, std.*
  from test_addresses as a,
       as_config cfg,
       LATERAL as_standardize(
            address,
            grammar,
            clexicon,
            a.locale,
            filter
        ) as std
 where a.countrycode=cfg.countrycode
 order by a.countrycode;

```

In this example, we ``order by a.countrycode`` so we only build the Lexicon and
Grammar once for the country. There are multiple slots in the cache, but it is
possible to overwrite a slot to make room for a new incoming slot in the cache
if it fills up and this prevents that from potentially becoming a problem.

You might also notice that the ``locale`` is save with the address and not the `Lexicon``. The reason is that many countries are multilingual so the specific interpertation of an address needs to be based on its locale. The Lexicon may contain words and phrases for multiple languages as does the US lexicon that has some Spanish and French words used in addresses in the US.

## Debugging Standardization Problems

It can be hard to understand the interplay between Lexicon and the Grammar.
There are two functions that can used to help understand this interplay. The
function ``as_standardize()`` takes your address, grammar, lexicon, etc. and
breaks the address into tokens and classifies the tokens. Next it takes the
tokens and searches the Grammar to find potentially multiple matches against
the Grammar. These matches are scored and the best score is the result.

So lets look at each of these. First we can look at the results of ``as_standardize()``. In the following query we have addresses in a table and select the address via ``a.id=93090``.

```
select a.id, std.*, dmetaphone(std.name)
  from rawdata.addresses as a,
       as_config as cfg,
       LATERAL as_standardize( address, grammar, clexicon, 'en_AU', filter) as std
 where countrycode='au' and dataset='gnaf'
   and a.id=93090;

  id   | building | house_num | predir | qual | pretype |    name    | suftype  | sufdir | ruralroute | extra |   city    | prov | country | postcode | box | unit |                                       pattern                                       | dmetaphone
-------+----------+-----------+--------+------+---------+------------+----------+--------+------------+-------+-----------+------+---------+----------+-----+------+-------------------------------------------------------------------------------------+------------
 93090 |          | 1         |        |      |         | POINT PARK | CRESCENT |        |            |       | DOCKLANDS | VIC  |         | 3008     |     |      | NUMBER WORD TYPE TYPE CITY PROV QUAD -> HOUSE STREET STREET SUFTYP CITY PROV POSTAL | PNTP
(1 row)

```

We recently added an output fields to as_standardize() called pattern the returns a string like:
```
NUMBER WORD TYPE CITY PROV QUAD -> HOUSE STREET SUFTYP CITY PROV POSTAL
```

This contains the list of tokens types on input and how they were standardized
on output. This is more of a diagnosic tool than anything else. It might be
interesting to use it to do some statistical analysis on what rules were used
in the grammar.

Next we might want to look at how the address was tokenized and how the tokens
were classified. If you are not happy with the way tokens are classified you
can make changes to the Lexicon and check the results here.

```
select a.id, std.*
  from rawdata.addresses as a,
       as_config as cfg,
       LATERAL as_parse( address, clexicon, 'en_AU', filter) as std
 where countrycode='au' and dataset='gnaf' and a.id=93090;

  id   | seq |   word    |     inclass      | attached
-------+-----+-----------+------------------+----------
 93090 |   1 | 1         | NUMBER           |
 93090 |   2 | POINT     | WORD,TYPE        |
 93090 |   3 | PARK      | WORD,TYPE,PLACEN |
 93090 |   4 | CRESCENT  | WORD,TYPE        |
 93090 |   5 | DOCKLANDS | WORD             |
 93090 |   6 | VIC       | WORD,PROV        |
 93090 |   7 | 3008      | NUMBER,QUAD      |
(7 rows)

```

You can put phrases in the lexicon the consist of multiple words, like a city
name, like ``NORTH CHELMSFORD``, this has the advantage the ``NORTH`` is
recognized as part of the city rather than as street name suffix. But tokens
are collected in a greedy way such the the longest tokens are collected first.
So if you have lexicon entries like: ``` AAA BBB BBB CCC ``` and you have a
text string like ``AAA DDD AAA BBB CCC`` this will get broken into tokens of
``AAA``, ``DDD``, ``AAA BBB``, and ``CCC``, so you never see the ``BBB CCC``
token. The code also splits multiple word tokens into individual tokens so you
will get a second set of tokens of ``AAA``, ``DDD``, ``AAA``, ``BBB``, ``CCC``
and both sets of tokens will get searched for in the grammar with the best
scoring one returned as the final result.

Given a set of tokens, these get enumerated into a collection of patterns, and
then each pattern is matched against the grammar. In fact a given pattern might
have multiple ways that it can match against the grammar and all of these are
extracted and scored. The follow query show the results of this matching and
the related score for each pattern. The score is the sum of the rule scores
divided by the number of rules matched for that pattern.

If you remove the distinct you will see many duplicate rows. These are caused
by the fact that there are multiple ways to match the grammar that have the
same score. Regardless, this should give you a hint to how the grammar is
working for a set of tokens. You can adjust the rule scores move a pattern to
the top of the list, but be aware that it might change other addresses in a
negative way lowering them.

```
select distinct a.id, std.*
  from rawdata.addresses as a,
       as_config as cfg,
       LATERAL as_match( address, grammar, clexicon, 'en_AU', filter) as std
 where countrycode='au' and dataset='gnaf'
       and a.id=93090
 order by score desc;

  id   |                                        tokens                                         |       score
-------+---------------------------------------------------------------------------------------+-------------------
 93090 | NUMBER TYPE TYPE TYPE WORD PROV QUAD -> HOUSE PRETYP STREET SUFTYP CITY PROV POSTAL   | 0.675714288439069
 93090 | NUMBER TYPE TYPE TYPE WORD PROV QUAD -> HOUSE STREET STREET SUFTYP CITY PROV POSTAL   | 0.668333331743876
 93090 | NUMBER TYPE TYPE TYPE WORD PROV NUMBER -> HOUSE PRETYP STREET SUFTYP CITY PROV POSTAL | 0.661428579262325
 93090 | NUMBER TYPE TYPE TYPE WORD PROV NUMBER -> HOUSE STREET STREET SUFTYP CITY PROV POSTAL | 0.651666671037674
 93090 | NUMBER TYPE TYPE WORD WORD PROV QUAD -> HOUSE STREET SUFTYP CITY CITY PROV POSTAL     |  0.65166666607062
 93090 | NUMBER TYPE WORD TYPE WORD PROV QUAD -> HOUSE PRETYP STREET SUFTYP CITY PROV POSTAL   | 0.645714291504451
 93090 | NUMBER TYPE TYPE TYPE WORD PROV QUAD -> HOUSE PRETYP STREET STREET CITY PROV POSTAL   | 0.638333340485891
 93090 | NUMBER TYPE TYPE WORD WORD PROV NUMBER -> HOUSE STREET SUFTYP CITY CITY PROV POSTAL   | 0.635000005364418
 93090 | NUMBER TYPE WORD TYPE WORD PROV NUMBER -> HOUSE PRETYP STREET SUFTYP CITY PROV POSTAL | 0.631428582327706
 93090 | NUMBER TYPE TYPE TYPE WORD PROV NUMBER -> HOUSE PRETYP STREET STREET CITY PROV POSTAL | 0.621666679779688
 93090 | NUMBER TYPE TYPE WORD WORD PROV QUAD -> HOUSE PRETYP STREET CITY CITY PROV POSTAL     | 0.621666674812635
 93090 | NUMBER WORD TYPE WORD WORD PROV QUAD -> HOUSE STREET SUFTYP CITY CITY PROV POSTAL     | 0.616666669646899
 93090 | NUMBER WORD WORD TYPE WORD PROV QUAD -> HOUSE STREET STREET SUFTYP CITY PROV POSTAL   | 0.616666664679845
 93090 | NUMBER TYPE TYPE WORD WORD PROV NUMBER -> HOUSE PRETYP STREET CITY CITY PROV POSTAL   | 0.605000014106433
 93090 | NUMBER TYPE TYPE WORD WORD PROV QUAD -> HOUSE STREET STREET CITY CITY PROV POSTAL     | 0.602000004053116
 93090 | NUMBER WORD TYPE WORD WORD PROV NUMBER -> HOUSE STREET SUFTYP CITY CITY PROV POSTAL   | 0.600000008940697
 93090 | NUMBER WORD WORD TYPE WORD PROV NUMBER -> HOUSE STREET STREET SUFTYP CITY PROV POSTAL | 0.600000003973643
 93090 | NUMBER TYPE WORD WORD WORD PROV QUAD -> HOUSE PRETYP STREET CITY CITY PROV POSTAL     | 0.586666678388914
 93090 | NUMBER TYPE WORD WORD WORD PROV QUAD -> HOUSE PRETYP STREET STREET CITY PROV POSTAL   |  0.58666667342186
 93090 | NUMBER TYPE TYPE WORD WORD PROV NUMBER -> HOUSE STREET STREET CITY CITY PROV POSTAL   | 0.582000011205673
 93090 | NUMBER TYPE WORD WORD WORD PROV NUMBER -> HOUSE PRETYP STREET CITY CITY PROV POSTAL   | 0.570000017682711
 93090 | NUMBER TYPE WORD WORD WORD PROV NUMBER -> HOUSE PRETYP STREET STREET CITY PROV POSTAL | 0.570000012715658
 93090 | NUMBER TYPE WORD WORD WORD PROV QUAD -> HOUSE STREET CITY CITY CITY PROV POSTAL       | 0.562000003457069
 93090 | NUMBER WORD WORD WORD WORD PROV QUAD -> HOUSE STREET STREET STREET CITY PROV POSTAL   |              0.55
 93090 | NUMBER TYPE WORD WORD WORD PROV NUMBER -> HOUSE STREET CITY CITY CITY PROV POSTAL     | 0.542000010609627
 93090 | NUMBER WORD WORD WORD WORD PROV QUAD -> HOUSE STREET STREET CITY CITY PROV POSTAL     | 0.540000003576279
 93090 | NUMBER WORD WORD WORD WORD PROV NUMBER -> HOUSE STREET STREET STREET CITY PROV POSTAL | 0.530000007152557
 93090 | NUMBER WORD WORD WORD WORD PROV NUMBER -> HOUSE STREET STREET CITY CITY PROV POSTAL   | 0.520000010728836
 93090 | NUMBER WORD WORD WORD WORD PROV QUAD -> HOUSE STREET CITY CITY CITY PROV POSTAL       | 0.520000007748604
 93090 | NUMBER WORD WORD WORD WORD PROV NUMBER -> HOUSE STREET CITY CITY CITY PROV POSTAL     | 0.500000014901161
(30 rows)
```

In general, localized changes (ie: related to a word or phrase) should be done
by adding to the Lexicon or changing how Lexicon entries are classified. For
global changes (ie: ones related to may addresses or a class of addresses)
making adjustments to the grammar is better. I typically look at 1,000s if not
1,000,000s of records when building a new Lexicon and Grammar.


