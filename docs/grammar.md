# Grammar

The grammar files descibe the *language* structure of an address. These are
user definable and to some extent need to be defined for each specific data set
you might be working with. The project provides a starter set of grammars for
various countries that might be good starting points for developing your own
grammar files.

## What do we mean by *language* structure?

For example, and address might be define as:

```
[ADDRESS]
house_number street city state postal country
street house_number postal city country
house_number street
```

These three simple example are only for explanation, and a real grammar would
need to account for things like optional fields, maybe apartment numbers, etc.

The first example might match many addresses in the US and Canada. The USPS has
a document Pub. 28 that defines valid address structure and acceptable
abbreviations the can be used to build the Lexicon.

The second example might be used at the top level to match some European
country addresses where the house number follows the streat name.

The third example might be used by someone that is only working with a small
subset of data covering a single city where there is no need to differentiate
between localities.

But this is only the top level of the grammar and and we need to describe each
term in more detail eventually down to the level of mapping
[InClass::Type](inclas.md) tokens to [OutClass::Type](outclass.md) token
classification.  For example, lets look at **house_number** in more detail.

```
[house_number]
NUMBER -> HOUSE -> 0.5
NUMBER FRACT -> HOUSE HOUSE -> 0.6
```

This is a simple definition of **house_number** and in this case we are looking
for a token classified as ``InClass::NUMBER`` and we want to map that token to
``OutClass::HOUSE`` and we assign a probability of 0.5 to that match. We could
also list additional token patterns that we wish to recognize as a
**house_number**. The second rule would match something like ``123 1/2`` as a
**house_number** also.

So from these two examples you should be able to see that we have two types of
rules in a grammar. In the ``[ADDRESS]`` example, each line represents a
alternative definitions for an address. To match ``[ADDRESS]``, we have to
match one or more of these lines. To match one of these lines, we have to match
all the referenced sections in that order and we must consume all the input
tokens.

The second example ``[house_number]`` has a different format and here we have
to consume input tokens, ie: the tokens before the first '->'.

The ``[ADDRESS]`` style section is referred to a *MetaSection* and the
``[house_number]`` section is referred to as a *RuleSection*. You can not mix
*MetaSection* lines with *RuleSection* lines.

## See Also

* [Lexicon](lexicon.md)
* [Token](token.md)
* [InClass::Type](inclass.md)
* [OutClass::Type](outclass.md)

