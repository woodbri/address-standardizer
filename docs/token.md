# Token

The tokenizer works with the Lexicon to split an address string into a string
of tokens that have an input classification either explicitly by virtue that
they are in the Lexicon or implicitly by the tokenizer. A Token is similar in
structure to the LexEntry object but carries additional information such is
whether or not the word was found in the lexicon and after input tokens have
been matched against the grammar what there output classification will be. For
example, *ST* can be classified as *TYPE* in the context of *STREET* or as
*WORD* in the context of *SAINT*. Only after the address tokens have been match
against the grammar are we able to determine which context is most likely to be
the context of usage for this particular address.

The Token object as the following fields:

* WORD - a word or phrase parsed from the address string
* STDWORD - a standardized version of *WORD* based on the Lexicon or *WORD*
* TYPELIST - a list of possible classifications
* OUTCLASS - the *TYPE* from the *TYPELIST* that was used in the matching against the grammar.
* ATTACHED - The value(s) from the Lexicon entries that matched *WORD*
* INLEX - Boolean value to indicate if *WORD* was found in the Lexicon.

The *STDWORD* and *OUTCLASS* values are only set after matching the input
tokens against the grammar, so they can be set based on the context of the
grammar that was matched.

## See Also

* [Overview](overview.md)
* [Lexicon](lexicon.md)
* [Grammar](grammar.md)
* [InClass::Type](inclass.md)
* [OutClass::Type](outclass.md)

