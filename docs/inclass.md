# InClass Types

The InClass types are how tokens get classified when an address string is parsed into tokens. These are used in the Lexicon to identify words or phrases as particular types of words. For example:

* STREET, ST, AVENUE, AVE, ROAD, RD could be classified as a **TYPE**
* SAINT, ST might be classified as a **WORD**

You might have noticed that *ST* can be classified as either **TYPE** or
**WORD** depending on its usage. Consider ``123 MAIN ST ST CLOUD FL``. This can
become confusing very fast. The key is that the Lexicon and the Grammar have to
work together to clasify tokens and then to match a stream of parsed tokens to
a set of grammar rules. In the [Lexicon](lexicon.md) a token may be classified
with multiple types as appropriate.

The list of input types is:

| InClass::Type | Description |
| ------------- | ----------- |
|   NUMBER    |  A string of digits.|
|   WORD      |  A word is a string of letters of arbitrary length. A single letter can be both a SINGLE and a WORD.|
|   TYPE      |  Words and abbreviation used to denote street typess. For example, 'ST' or 'AVE'.|
|   QUALIF    |  Words that qualify a street name, like, 'OLD', 'EXTENSION', 'BUSINESS', 'ALTERNATE', etc.|
|   ROAD      |  Words and abbreviations used to denote highways and roads. Example: the 'Interstate' in 'Interstate 5'.|
|   STOPWORD  |  A word with low lexical significance, that can be omitted in parsing. For example, 'THE'. STOPWORDS combine with WORDS. In rules a string of multiple WORDs and STOPWORDs will be represented by a single WORD token.|
|   RR        |  Words and abbreviations used to denote rural routes. 'RR'.|
|   DASH      |  A hyphen|
|   CITY      |  City name|
|   PROV      |  State, province, or region name or abbreviation|
|   NATION    |  Nation name or abbreviation|
|   AMPERS    |  The ampersand (&) is frequently used to abbreviate the word 'and'.|
|   BOXH      |  Words used to denote post office boxes. For example 'Box' or 'PO Box'.|
|   ORD       |  Representations such as First or 1st. Often used in street. Are often standardized as numbers.|
|   UNITH     |  Words and abbreviation used to denote internal subaddresses. For example, 'APT' or 'UNIT'.|
|   UNITT     |  Which UNIT is being described|
|   SINGLE    |  A single letter.|
|   BUILDH    |  Words used to denote buildings or building complexes, usually as a prefix. For example 'Tower' in 'Tower 7A'.|
|   MILE      |  Words used to denote milepost addresses.|
|   DOUBLE    |  A sequence of two letters. Often used as identifiers.|
|   DIRECT    |  Words used to denote directions, for example 'North'. These should get standardized to the full word not the abreviation.|
|   MIXED     |  An alphanumeric string that contains both letters and digits. Used for identifiers.|
|   BUILDT    |  Words and abbreviations used to denote buildings or building complexes, usually as a suffix. For example, 'Shopping Centre'.|
|   FRACT     |  Fractions are sometimes used in civic numbers or unit numbers.|
|   PCT       |  A 3 character sequence of number letter number. Identifies an LDU, the last 3 characters of a Canadian postal code.|
|   PCH       |  A 3 character sequence of letter number letter. Identifies an FSA, the first 3 characters of a Canadian postal code.|
|   QUINT     |  A 5 digit number. Identifies a Zip Code|
|   QUAD      |  A 4 digit number. Identifies ZIP4.|
|   PUNCT     |  Punctuation character(s)|
|   SPACE     |  whitespace|
|   PLACEN    |  Placename keyword|
|   EMDASH    |  Token used to identify where a word was split by the tokenizer|
|   SLASH     |  A slash '/' token like 1/a 1/ab, but not a FRACT|
|   ATSIGN    |  a at sign '@' token to indicate an intersection|
|   BADTOKEN  |  A bad token type.|

# Attachment Types

| InClass::AttachType | Description |
| ------------------- | ----------- |
| DETACHED  |  Used it indicate no attachement |
| DET_PRE   |  Detached prefix |
| DET_SUF   |  Detached suffix |
| ATT_PRE   |  Attached prefix |
| ATT_SUF   |  Attached suffix |

You can use *DETACHED* or leave the field empty to indicate that the the token is not attached. *DET_PRE*, *DET_SUF*, *ATT_PRE*, *ATT_SUF* can be placed in a comma separated list ass appropriate to indicate the possible attachments for the token. For example *STRASSE* could have *DET_SUF,ATT_SUF* associated with it to indcate that it follows the street name and might be attached or not.


## See Also

* [Overview](overview.md)
* [Lexicon](lexicon.md)
* [Grammar](grammar.md)
* [Token](token.md)
* [OutClass::Type](outclass.md)

