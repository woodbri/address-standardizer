# Lexicon

The Lexicon is a dictionary of words and phrases that might be found in an
addresses. It is used when the address is broken into tokens to help classify
the tokens. For example, we might have *ST* to recognize *STREET* or *SAINT*,
We can create two entries in the lexicon for *ST*, one to classify it as *TYPE*
with a standardization of *STREET*, and another to classify is as *WORD* for
*SAINT*. The purpose of the lexicon to to help identify parts of an address by
their classification so that we can sort our the tokens using the address
grammar.

Each entry in the Lexicon contains a word or phrase, as standardization of the
word or phrase, a set of possible classifications that might be used for that
word and information about whether the word is attached to another word. The
possible classifications are defined in [InClass::Type](inclass.md). We do not
and should not try to define all words, the tokenizer will assign default
classifications to words that are not in the Lexicon.

Attached words only appear in German and this allows us to split attached words
from the base word. For example, *BRICKHAUSSTRASSE* can be recognized and split
into *BRICKHAUS* and *STRASSE* if *STRASSE* is defined as *ATT_SUF*. See
[InClass::Type](inclass.md) for a list of attachment types.

In general you want to keep the Lexicon as small as possible for performance in
loading, but you need enough entries to classify various key tokens in the
address so it can match a grammar with some confidence. To get a better idea
about this let look at the case of an empty lexicon. In this case the tokenizer
will assign the default classification of *WORD* to most tokens. In reality
there are a bunch of default tokens but they do not provide much useful
classification by themselves. So an address like: *123 green brier st anytown
mystate 12345* could get parsed into ``NUMBER WORD WORD WORD WORD WORD
NUMBER``. Given this we might be able to assume that the first ``NUMBER`` is
the house number and the last ``NUMBER`` is a postal code, but we would be hard
pressed to decide which of the ``WORD`` tokens belonged to ``STREET CITY
STATE`` parts of the address. In this example we could classify *brier* as
``WORD`` but this adds no value over letting the tokenizer do that for us. On
the other hand we might want to classify *green* as both a *WORD* and a *TYPE*
so we can capture it as *WORD* in the example above, as a street *TYPE* when
used as ``123 college green``.

## Lexicon File Format

The Lexicon file format is lines where fields on the lines are separated by tab
characters and the lines end with &lt;CR>&lt;LF> or &lt;LF>. The first line is a header
followed by lexicon entries.

### Format
```
LEXICON:<tab>NAME<tab>LANG<tab>LOCALE<tab>IGNORED<LF>
LEXENTRY:<tab>WORD<tab>STDWORD<tab>TYPELIST<tab>ATTACHLIST<LF>
LEXENTRY:<tab>WORD<tab>STDWORD<tab>TYPELIST<tab>ATTACHLIST<LF>
...
```
Where the *LEXICON:* line is the header that must be the first line and contains the fields:

* NAME - is an arbitrary name given the lexicon (currently not used)
* LANG - is an [InClass::Lang](inclass.md) (currently not used)
* LOCALE - is an ISO Locale string that represents the bulk of the Lexicon entries (currently not used)
* IGNORED - leave this blank

The idea behind defining these is that for a future enhancement, we might be
able to autodetect the language of the address and then select the appropriate
Lexicon and Grammar based on that, so it is advisable to set these fields to
reasonable values.

The *LEXENTRY:* lines contain the Lexicon entries with the following fields:

* WORD - a word or phrase that is being added to the lexicon
* STDWORD - a standardized version of *WORD*
* TYPELIST - a comma separated list of [InClass::Type](inclass.md) used as potential classifications for this *WORD* - *STDWORD* combination.
* ATTACHLIST - For most entries this can be *DETACHED* or simply left empty. See [InClass::Type](inclass.md) for a list of attachment types.

The github repository contains a collection of lexicons and grammars in ``data/sample/`` directory.

## See Also

* [Overview](overview.md)
* [Grammar](grammar.md)
* [Token](token.md)
* [InClass::Type](inclass.md)
* [OutClass::Type](outclass.md)
