# OutClass Types

The goal of the address standardizer is to parse an input address string into token, then match those tokens to a grammar and based on that matching reclassify or assign the tokens to the OutClass Type fields. You can think of the this as a table with the column names as the OutClass Types.

For example lets look at the following address ``123 MAIN ST ANYTOWN MA 12345 USA``:

| Step | 1 | 2  | 3 | 4     | 5 | 6   | 7 |
| ---- | - | -- | - | ----- | - | --- | - |
| Tokens | 123 | MAIN | ST | ANYTOWN | MA | 12345 | USA |
| InClass | NUMBER | WORD | TYPE,WORD | WORD | PROV | NUMBER,QUINT | NATION |
| OutClass | HOUSE | STREET | SUFTYP | CITY | PROV | POSTAL | NATION |

Here we parse the address into Tokens in line 1, and they get classified by the type of Token it is as seen in the second line, and after we match the tokens against a grammar and reclassify them we get the OutClass classifications on line three.

The full list of OutClass types is listed in the following table.

| OutClass Type | Description |
| ------------- | ----------- |
| BLDNG   | Unparsed building identifiers and types. |
| HOUSE   | The civic address number. The '123' in '123 MAIN ST' |
| PREDIR  | A directional modifier that precedes the street name. Example, the 'WEST' in '3715 TENTH AVENUE WEST'. |
| QUALIF  | A pre or post modifier. Example, the 'OLD' in '3715 OLD HIGHWAY 99' or 'BUSINESS' in '1203 HIGHWAY 45 BUSINESS'. |
| PRETYP  | A street type preceding the root street name. Example, the 'HIGHWAY' in '3715 HIGHWAY 99'. |
| STREET  | The root street name, stripped of directional or type modifiers. Example, the 'TENTH' in '3715 WEST TENTH AVENUE'. |
| SUFTYP  | A street type following the root street name. Example, the 'AVENUE' in '3715 WEST TENTH AVENUE'. |
| SUFDIR  | A directional modifier that follows the street name. Example, the 'WEST' in '3715 TENTH AVENUE WEST'. |
| RR      | The 'RR' in 'RR 7' |
| EXTRA   | An otherwise unclassified output. |
| CITY    | A placename. Example, 'LONDON' |
| PROV    | A state, province or region. Example, 'NY' or 'NEW YORK'. |
| NATION  | A country name or abbreviation. |
| POSTAL  | A postal code component. |
| BOXH    | The 'BOX' in 'BOX 3B' |
| BOXT    | The '3B' in 'BOX 3B' |
| UNITH   | The 'APT' in 'APT 3B' |
| UNITT   | The '3B' in 'APT 3B' |
| BADTOKEN | bad token type |


## See Also

* [Lexicon](lexicon.md)
* [Grammar](grammar.md)
* [Token](token.md)
* [InClass::Type](inclass.md)

