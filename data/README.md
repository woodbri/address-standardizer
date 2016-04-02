# Organization of Lexicons and Grammars

Lexicons and Grammars are organized by datasets. The reason for this is the
fact each dataset will have its own peculiarities that need to be addressed in
the lexicon and grammars for that dataset. Look at the *sample* dataset for a
good starting point to create a dataset specific set of files. Files should be
organized using this schema ``data/&lt;dataset>/&lg;country>.[lex|gmr]``, there
is a large collection of initial lexicons and grammars in
``data/sample/&lg;country>.[lex|gmr]``.

Some datasets like the US Census Tiger data or the Canadian geobase or Statcan
data will only need a single lexicon and grammar files. Other datasets that
have coverage for multiple countries will likely need lexicon and grammar files
constructed for each country.

## What is the Purpose of the Lexicon and Grammar Files

Given an address string of text, our goal is to break this into some number of
components the we can classify as being house number, street name, the city,
province, postal code and country. Some of these fields may not be needed and
some of them can be broken down into sub-components, like the street name might
get separated into a base name and street type components and maybe other
sub-components like a qualifier or street directional indicator.

We do this because we want to take our reference street data and standardize
that, so we can them take the our in coming address and standardize that, so
when we search for the input address in the standardize reference set we are
hopefully matching apples to apples and not apples to oranges.

While this could be considered a natural language processing task, we don't
care about getting getting the language *correct*. So for example, think about
articles like a, an, the, etc. in English or de, del, de la, etc. in French or
Spanish, while these words might be important in the structure of a language,
they add little or no value in parsing and address. For example, if an article
exists does it help me differentiate that the following word is street name or
a city name? In fact, if all articles were removed would is prevent me from
parse the string into components.

There will be cases where a word might have multiple meanings depending on the
context that it is used in. In this case, we can give multiple classifications
to a word and let the grammar select the appropriate classification based on
the context in which it is used. For example, *ST* might be an abbreviation for
*STREET* or *SAINT*.

```
ST ALBINS ST    -- for Saint Albins Street
```

In this case we want the first *ST* to be classified as a *WORD* and the second
*ST* classified as *TYPE*. So in the lexicon we could classify *ST* as
*WORD,TYPE* and the problem is solved assuming we have grammar rule like ``WORD
WORD TYPE``.

There can be many ambiguities where between the lexicon and the grammar, there
are multiple possible solution. In the grammar you will notice each Rule has a
score. You can think of the score as a probability that this rule is the one we
want. Initially set these all to 0.5, then as you test your grammar against
addresses and find that the wrong rule is getting selected increase the score
on the correct rule or decrease the score on the wrong rule.

Remember that the goal, is be able to parse addresses into a string of
components and that any string of similar components could be considered a
likely match.

## How To Construct New Lexicon or Grammar Files

The process is fairly straight forward and can be applied to most any country.
The purpose of the lexicon is to identify tokens in an address as parts of a
grammar for that address. These identified tokens then help the grammar decide
which part of the address belongs to the house number, the street name, the
city, the postal code or other parts of an address. Depending on your use case,
and what you incoming data looks like you can make this as simple or as complex
as you need. For example, if your addresses will never have apartment phrases
included in them, then there is no need to add that to the lexicon and grammar.
If you are only geocoding for a single country, then there is no need to
require the country be specified, but on the other hand, if you geocoding
will be handling addresses for example in all of Europe, you will need to make
sure that the country is specified.

## The Process

Start simple and add lexicon entries and rules as you need to develop the
parser for your dataset.

1. Look at a *sample* grammar file for your country or one similar to your country. Also checkout [Universal Postal Union](http://www.upu.int/en/activities/addressing/postal-addressing-systems-in-member-countries.html) for the country you are interested in.
2. Build your grammar file first and keep it as simple as possible. You can add to it as you find new cases that you need to recognize.
3. Start building your lexicon. Looking at your grammar, what tokens do you need to identify, for example, provinces, *PROV*, nation, *NATION*, street types, *TYPE*. Find lists of this words and add them to your lexicon. You might want to add a list of articles as *IGNORE*, to ignore them. Is it common to write numbers out as words versus digits, then you might want to include entires in the lexicon to map these back to number, like *ELEVEN* to *11* for example.
4. I find it useful looks over distinct lists of street names for common types or features that might help identify part of the street name.
5. Standard your reference dataset into a new table with foreign key back to the origin record. Then get a list of all records the failed to standardize and assess what you need to do to added or modify the grammar or lexicon and repeat until you are satisfied.
6. Generate a pull request to submit your files back to the project.
