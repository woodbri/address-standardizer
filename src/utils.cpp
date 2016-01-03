/*HEADER*
*HEADER*/

#include "utils.h"

using icu::UnicodeSet;
using icu::UnicodeString;

std::string Utils::unaccentUTF8( const std::string &str ) {
    // UTF-8 std::string -> UTF-16 UnicodeString
    UnicodeString source = UnicodeString::fromUTF8(StringPiece(str));

    // Transliterate UTF-16 UnicodeString
    UErrorCode status = U_ZERO_ERROR;
    Transliterator *accentsConverter = Transliterator::createInstance(
        "NFD; [:M:] Remove; NFC", UTRANS_FORWARD, status);
    accentsConverter->transliterate(source);
    // TODO: handle errors with status

    // UTF-16 UnicodeString -> UTF-8 std::string
    std::string result;
    source.toUTF8String(result);

    return result;
}


std::string Utils::upperCaseUTF8( const std::string &str, const std::string loc ) {
    // UTF-8 std::string -> UTF-16 UnicodeString
    UnicodeString source = UnicodeString::fromUTF8(StringPiece(str));

    Locale locale(loc.c_str());

    // uppercase
    source.toUpper(locale);

    // UTF-16 UnicodeString -> UTF-8 std::string
    std::string result;
    source.toUTF8String(result);

    return result;
}


std::string normalizeUTF8( const std::string &str, UErrorCode &errorCode) {
    std::string result("ERROR");

    // UTF-8 std::string -> UTF-16 UnicodeString
    UnicodeString source = UnicodeString::fromUTF8(StringPiece(str));

    errorCode = U_ZERO_ERROR;

    // get instance of normalizer
    const Normalizer2 &norm(*Normalizer2::getNFCInstance( errorCode ));
    if ( U_FAILURE(errorCode) )
        return result;

    errorCode = U_ZERO_ERROR;

    // normalize the string
    source = norm.normalize(source, errorCode);
    if ( U_FAILURE(errorCode) )
        return result;

    // UTF-16 UnicodeString -> UTF-8 std::string
    source.toUTF8String(result);

    return result;
}


/**
\param locale - is a ISO-639  2-3 charactr string denoting the region

\ref http://demo.icu-project.org/icu-bin/locexp?d_=en
\ref http://userguide.icu-project.org/locale

af  - Afrikaans (Namibia),  Afrikaans (South Africa)
agq - Aghem (Cameroon)
ak  - Akan (Ghana)
sq  - Albanian (Albania),  Albanian (Kosovo),  Albanian (Macedonia)
am  - Amharic (Ethiopia)
ar  - Arabic
hy  - Armenian (Armenia)
as  - Assamese (India)
asa - Asu (Tanzania)
az  - Azerbaijani (Cyrillic),  Azerbaijani (Latin)
az_Cyrl - Azerbaijani (Cyrillic, Azerbaijan)
az_Latn - Azerbaijani (Latin, Azerbaijan)
ksf - Bafia (Cameroon)
bm  - Bambara (Mali)
bas - Basaa (Cameroon)
eu  - Basque (Spain)
be  - Belarusian (Belarus)
bem - Bemba (Zambia)
bez - Bena (Tanzania)
bn  - Bengali (Bangladesh),  Bengali (India)
brx - Bodo (India)
bs  - Bosnian (Cyrillic),  Bosnian (Latin)
bs_Cyrl - Bosnian (Cyrillic, Bosnia & Herzegovina)
bs_Latn - Bosnian (Latin, Bosnia & Herzegovina)
br  - Breton (France)
bg  - Bulgarian (Bulgaria)
my  - Burmese (Myanmar [Burma])
ca  - Catalan (Andorra),  Catalan (France),  Catalan (Italy),  Catalan (Spain)
tzm - Central Atlas Tamazight (Morocco)
ce  - Chechen (Russia)
chr - Cherokee (United States)
cgg - Chiga (Uganda)
zh  - Chinese (Simplified),  Chinese (Traditional)
zh_Hans - Chinese (Simplified, China),  Chinese (Simplified, Hong Kong SAR China),  Chinese (Simplified, Macau SAR China),  Chinese (Simplified, Singapore)
zh_Hant - Chinese (Traditional, Hong Kong SAR China),  Chinese (Traditional, Macau SAR China),  Chinese (Traditional, Taiwan)
ksh - Colognian (Germany)
kw  - Cornish (United Kingdom)
hr  - Croatian (Bosnia & Herzegovina),  Croatian (Croatia)
cs  - Czech (Czech Republic)
da  - Danish (Denmark),  Danish (Greenland)
dua - Duala (Cameroon)
nl  - Dutch (Aruba),  Dutch (Belgium),  Dutch (Caribbean Netherlands),  Dutch (Curaçao),  Dutch (Netherlands),  Dutch (Sint Maarten),  Dutch (Suriname)
dz  - Dzongkha (Bhutan)
ebu - Embu (Kenya)
en  - English
eo  - Esperanto
et  - Estonian (Estonia)
ee  - Ewe (Ghana),  Ewe (Togo)
ewo - Ewondo (Cameroon)
fo  - Faroese (Denmark),  Faroese (Faroe Islands)
fil - Filipino (Philippines)
fi  - Finnish (Finland)
fr  - French
fur - Friulian (Italy)
ff  - Fulah (Cameroon),  Fulah (Guinea),  Fulah (Mauritania),  Fulah (Senegal)
gl  - Galician (Spain)
lg  - Ganda (Uganda)
ka  - Georgian (Georgia)
de  - German (Austria),  German (Belgium),  German (Germany),  German (Liechtenstein),  German (Luxembourg),  German (Switzerland)
el  - Greek (Cyprus),  Greek (Greece)
gu  - Gujarati (India)
guz - Gusii (Kenya)
ha  - Hausa (Ghana),  Hausa (Niger),  Hausa (Nigeria)
haw - Hawaiian (United States)
he  - Hebrew (Israel)
hi  - Hindi (India)
hu  - Hungarian (Hungary)
is  - Icelandic (Iceland)
ig  - Igbo (Nigeria)
smn - Inari Sami (Finland)
id  - Indonesian (Indonesia)
ga  - Irish (Ireland)
it  - Italian (Italy),  Italian (San Marino),  Italian (Switzerland)
ja  - Japanese (Japan)
dyo - Jola-Fonyi (Senegal)
kea - Kabuverdianu (Cape Verde)
kab - Kabyle (Algeria)
kkj - Kako (Cameroon)
kl  - Kalaallisut (Greenland)
kln - Kalenjin (Kenya)
kam - Kamba (Kenya)
kn  - Kannada (India)
ks  - Kashmiri (India)
kk  - Kazakh (Kazakhstan)
km  - Khmer (Cambodia)
ki  - Kikuyu (Kenya)
rw  - Kinyarwanda (Rwanda)
kok - Konkani (India)
ko  - Korean (North Korea),  Korean (South Korea)
khq - Koyra Chiini (Mali)
ses - Koyraboro Senni (Mali)
nmg - Kwasio (Cameroon)
ky  - Kyrgyz (Kyrgyzstan)
lkt - Lakota (United States)
lag - Langi (Tanzania)
lo  - Lao (Laos)
lv  - Latvian (Latvia)
ln  - Lingala (Angola),  Lingala (Central African Republic),  Lingala (Congo - Brazzaville),  Lingala (Congo - Kinshasa)
lt  - Lithuanian (Lithuania)
dsb - Lower Sorbian (Germany)
lu  - Luba-Katanga (Congo - Kinshasa)
luo - Luo (Kenya)
lb  - Luxembourgish (Luxembourg)
luy - Luyia (Kenya)
mk  - Macedonian (Macedonia)
jmc - Machame (Tanzania)
mgh - Makhuwa-Meetto (Mozambique)
kde - Makonde (Tanzania)
mg  - Malagasy (Madagascar)
ms  - Malay (Brunei),  Malay (Malaysia),  Malay (Singapore)
ml  - Malayalam (India)
mt  - Maltese (Malta)
gv  - Manx (Isle of Man)
mr  - Marathi (India)
mas - Masai (Kenya),  Masai (Tanzania)
mzn - Mazanderani (Iran)
mer - Meru (Kenya)
mgo - Metaʼ(Cameroon)
mn  - Mongolian (Mongolia)
mfe - Morisyen (Mauritius)
mua - Mundang (Cameroon)
naq - Nama (Namibia)
ne  - Nepali (India),  Nepali (Nepal)
nnh - Ngiemboon (Cameroon)
jgo - Ngomba (Cameroon)
nd  - North Ndebele (Zimbabwe)
lrc - Northern Luri (Iran),  Northern Luri (Iraq)
se  - Northern Sami (Finland),  Northern Sami (Norway),  Northern Sami (Sweden)
nb  - Norwegian Bokmål (Norway),  Norwegian Bokmål (Svalbard & Jan Mayen)
nn  - Norwegian Nynorsk (Norway)
nus - Nuer (South Sudan)
nyn - Nyankole (Uganda)
or  - Oriya (India)
om  - Oromo (Ethiopia),  Oromo (Kenya)
os  - Ossetic (Georgia),  Ossetic (Russia)
ps  - Pashto (Afghanistan)
fa  - Persian (Afghanistan),  Persian (Iran)
pl  - Polish (Poland)
pt  - Portuguese (Angola),  Portuguese (Brazil),  Portuguese (Cape Verde),  Portuguese (Guinea-Bissau),  Portuguese (Macau SAR China),  Portuguese (Mozambique),  Portuguese (Portugal),  Portuguese (São Tomé & Príncipe),  Portuguese (Timor-Leste)
pa  - Punjabi (Arabic),  Punjabi (Gurmukhi)
pa_Guru - Punjabi (Gurmukhi, India)
qu  - Quechua (Bolivia),  Quechua (Ecuador),  Quechua (Peru)
ro  - Romanian (Moldova),  Romanian (Romania)
rm  - Romansh (Switzerland)
rof - Rombo (Tanzania)
rn  - Rundi (Burundi)
ru  - Russian (Belarus),  Russian (Kazakhstan),  Russian (Kyrgyzstan),  Russian (Moldova),  Russian (Russia),  Russian (Ukraine)
rwk - Rwa (Tanzania)
sah - Sakha (Russia)
saq - Samburu (Kenya)
sg  - Sango (Central African Republic)
sbp - Sangu (Tanzania)
gd  - Scottish Gaelic (United Kingdom)
seh - Sena (Mozambique)
sr  - Serbian (Cyrillic),  Serbian (Latin)
sr_Cyrl - Serbian (Cyrillic, Bosnia & Herzegovina),  Serbian (Cyrillic, Kosovo),  Serbian (Cyrillic, Montenegro),  Serbian (Cyrillic, Serbia)
sr_Latn - Serbian (Latin, Bosnia & Herzegovina),  Serbian (Latin, Kosovo),  Serbian (Latin, Montenegro),  Serbian (Latin, Serbia)
ksb - Shambala (Tanzania)
sn  - Shona (Zimbabwe)
ii  - Sichuan Yi (China)
si  - Sinhala (Sri Lanka)
sk  - Slovak (Slovakia)
sl  - Slovenian (Slovenia)
xog - Soga (Uganda)
so  - Somali (Djibouti),  Somali (Ethiopia),  Somali (Kenya),  Somali (Somalia)
es  - Spanish (Argentina),  Spanish (Bolivia),  Spanish (Canary Islands),  Spanish (Ceuta & Melilla),  Spanish (Chile),  Spanish (Colombia),  Spanish (Costa Rica),  Spanish (Cuba),  Spanish (Dominican Republic),  Spanish (Ecuador),  Spanish (El Salvador),  Spanish (Equatorial Guinea),  Spanish (Guatemala),  Spanish (Honduras),  Spanish (Latin America),  Spanish (Mexico),  Spanish (Nicaragua),  Spanish (Panama),  Spanish (Paraguay),  Spanish (Peru),  Spanish (Philippines),  Spanish (Puerto Rico),  Spanish (Spain),  Spanish (United States),  Spanish (Uruguay),  Spanish (Venezuela)
zgh - Standard Moroccan Tamazight (Morocco)
sw  - Swahili (Congo - Kinshasa),  Swahili (Kenya),  Swahili (Tanzania),  Swahili (Uganda)
sv  - Swedish (Åland Islands),  Swedish (Finland),  Swedish (Sweden)
gsw - Swiss German (France),  Swiss German (Liechtenstein),  Swiss German (Switzerland)
shi - Tachelhit (Latin),  Tachelhit (Tifinagh)
shi_Latn - Tachelhit (Latin, Morocco)
shi_Tfng - Tachelhit (Tifinagh, Morocco)
dav - Taita (Kenya)
ta  - Tamil (India),  Tamil (Malaysia),  Tamil (Singapore),  Tamil (Sri Lanka)
twq - Tasawaq (Niger)
te  - Telugu (India)
teo - Teso (Kenya),  Teso (Uganda)
th  - Thai (Thailand)
bo  - Tibetan (China),  Tibetan (India)
ti  - Tigrinya (Eritrea),  Tigrinya (Ethiopia)
to  - Tongan (Tonga)
tr  - Turkish (Cyprus),  Turkish (Turkey)
uk  - Ukrainian (Ukraine)
hsb - Upper Sorbian (Germany)
ur  - Urdu (India),  Urdu (Pakistan)
ug  - Uyghur (China)
uz  - Uzbek (Arabic),  Uzbek (Cyrillic),  Uzbek (Latin)
uz_Arab - Uzbek (Arabic, Afghanistan)
ur_Cyrl - Uzbek (Cyrillic, Uzbekistan)
ur_Latn - Uzbek (Latin, Uzbekistan)
vai - Vai (Latin),  Vai (Vai)
vai_Latn - Vai (Latin, Liberia)
vai_Vaii - Vai (Vai, Liberia)
vi  - Vietnamese (Vietnam)
vun - Vunjo (Tanzania)
wae - Walser (Switzerland)
cy  - Welsh (United Kingdom)
fy  - Western Frisian (Netherlands)
yav - Yangben (Cameroon)
yi  - Yiddish (World)
yo  - Yoruba (Benin),  Yoruba (Nigeria)
dje - Zarma (Niger)
zu  - Zulu (South Africa)
**/

