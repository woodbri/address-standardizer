


#include "utf8iterator.h"


const unsigned char kFirstBitMask = 128; // 1000000
const unsigned char kSecondBitMask = 64; // 0100000
const unsigned char kThirdBitMask = 32; // 0010000
const unsigned char kFourthBitMask = 16; // 0001000
const unsigned char kFifthBitMask = 8; // 0000100



Utf8Iterator::Utf8Iterator(std::string::const_iterator it) :
mStringIterator(it),
mCurrentValue(0),
mDirty(true)
{
}



Utf8Iterator::Utf8Iterator(const Utf8Iterator& source) :
mStringIterator(source.mStringIterator),
mCurrentValue(source.mCurrentValue),
mDirty(source.mDirty)
{
}



std::string Utf8Iterator::substr( Utf8Iterator &to ) {
    return std::string( mStringIterator, to.mStringIterator );
}



Utf8Iterator& Utf8Iterator::operator=(const Utf8Iterator& rhs)
{
    mStringIterator = rhs.mStringIterator;
    mCurrentValue = rhs.mCurrentValue;
    mDirty = rhs.mDirty;

    return *this;
}



Utf8Iterator::~Utf8Iterator()
{
}


// TODO checks for invalid UTF-8 syntax and throw exceptions 
Utf8Iterator& Utf8Iterator::operator++()
{
    char firstByte = *mStringIterator;
 
    std::string::difference_type offset = 1;
 
    // This means the first byte has a value greater than 127,
    //  and so is beyond the ASCII range.
    if ( firstByte & kFirstBitMask ) {

        // This means that the first byte has a value greater than 224,
        //  and so it must be at least a three-octet code point.
        if ( firstByte & kThirdBitMask ) {

            // This means that the first byte has a value greater than 240,
            // and so it must be a four-octet code point.
            if ( firstByte & kFourthBitMask )
                offset = 4;
            else
                offset = 3;
        }
        else {
            offset = 2;
        }
    }
 
    mStringIterator += offset;
 

    return *this;
}


 
Utf8Iterator Utf8Iterator::operator++(int)
{
    Utf8Iterator temp = *this;
    ++(*this);
    return temp;
}


 
// TODO checks for invalid UTF-8 syntax and throw exceptions 
Utf8Iterator& Utf8Iterator::operator--()
{
    --mStringIterator;
 
    // This means that the previous byte is not an ASCII character.
    if ( *mStringIterator & kFirstBitMask )
    {
        --mStringIterator;
        if ( ( *mStringIterator & kSecondBitMask ) == 0 ) {
            --mStringIterator;
            if ( ( *mStringIterator & kSecondBitMask ) == 0 ) {
                --mStringIterator;
            }
        }
    }

    mDirty = true;

    return *this;
}


 
Utf8Iterator Utf8Iterator::operator--(int)
{
    Utf8Iterator temp = *this;
    --(*this);
    return temp;
}



void Utf8Iterator::CalculateCurrentCodePoint() const
{
    if ( not mDirty ) return;

    char32_t codePoint = 0;
 
    char firstByte = *mStringIterator;
 
    // This means the first byte has a value greater than 127,
    // and so is beyond the ASCII range.
    if ( firstByte & kFirstBitMask ) {
        // This means that the first byte has a value greater than 191,
        // and so it must be at least a three-octet code point.
        if ( firstByte & kThirdBitMask ) {
            // This means that the first byte has a value greater than 224,
            // and so it must be a four-octet code point.
            if ( firstByte & kFourthBitMask ) {
                codePoint = ( firstByte & 0x07 ) << 18;
                char secondByte = *( mStringIterator + 1 );
                codePoint +=  ( secondByte & 0x3f ) << 12;
                char thirdByte = *( mStringIterator + 2 );
                codePoint +=  ( thirdByte & 0x3f ) << 6;;
                char fourthByte = *( mStringIterator + 3 );
                codePoint += ( fourthByte & 0x3f );
            }
            else {
                codePoint = ( firstByte & 0x0f ) << 12;
                char secondByte = *( mStringIterator + 1 );
                codePoint += ( secondByte & 0x3f ) << 6;
                char thirdByte = *( mStringIterator + 2 );
                codePoint +=  ( thirdByte & 0x3f );
            }
        }
        else {
            codePoint = ( firstByte & 0x1f ) << 6;
            char secondByte = *( mStringIterator + 1 );
            codePoint +=  ( secondByte & 0x3f );
        }
    }
    else {
        codePoint = firstByte;
    }

    mCurrentValue = codePoint;
}



char32_t Utf8Iterator::operator*() const
{
    CalculateCurrentCodePoint();

    return mCurrentValue;
}



bool Utf8Iterator::operator==(const Utf8Iterator& rhs) const
{
    return mStringIterator ==  rhs.mStringIterator;
}




bool Utf8Iterator::operator!=(const Utf8Iterator& rhs) const
{
    return mStringIterator !=  rhs.mStringIterator;
}



bool Utf8Iterator::operator==(std::string::iterator rhs) const
{
    return mStringIterator ==  rhs;
}



bool Utf8Iterator::operator==(std::string::const_iterator rhs) const
{
    return mStringIterator ==  rhs;
}



bool Utf8Iterator::operator!=(std::string::iterator rhs) const
{
    return mStringIterator !=  rhs;
}



bool Utf8Iterator::operator!=(std::string::const_iterator rhs) const
{
    return mStringIterator !=  rhs;
}



// ------------------------------------------------------------------
// non-class utility function(s)
// ------------------------------------------------------------------


std::string CodePointToUtf8( char32_t codePoint )
{
    if ( codePoint >= 0x110000 )
        throw std::runtime_error( "Invalid-Unicode-Code-Point" );
 
    std::string utf8Bytes;
 
    if ( codePoint < 0x80 ) {

        char c = static_cast<char>( codePoint );
        utf8Bytes.push_back( c );
    }
    else if ( codePoint < 0x800 ) {

        // The code point has the binary format 00000yyy yyxxxxxx.
        // It must be turned into two bytes: (110yyyyy, 10xxxxxx)
        utf8Bytes.reserve(2);
        // This is 110yyyyy
        char c1 =  static_cast<char>(0xc0 | (codePoint >> 6));
        utf8Bytes.push_back(c1);
        // This is 10xxxxxx
        char c2 =  static_cast<char>((codePoint) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c2);
    }
    else if ( codePoint < 0x10000 ) {

        // The code point has the binary format zzzzyyyy yyxxxxxx.
        // It must be turned into three bytes: (1110zzzz, 10yyyyyy, 10xxxxxx)
        utf8Bytes.reserve(3);
        // This is 1110zzzz
        char c1 =  static_cast<char>(0xe0 | (codePoint >> 12));
        utf8Bytes.push_back(c1);
        // This is 10yyyyyy
        char c2 =  static_cast<char>((codePoint >> 6) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c2);
        // This is 10xxxxxx
        char c3 =  static_cast<char>((codePoint) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c3);
    }
    else if ( codePoint < 0x110000 ) {

        // The code point has the binary format 000wwwzz zzzzyyyy yyxxxxxx.
        // It must be turned into four bytes: (11110www, 10zzzzzz, 10yyyyyy, 10xxxxxx)
        utf8Bytes.reserve(4);
        // This is 11110www
        char c1 =  static_cast<char>(0xf0 | (codePoint >> 18));
        utf8Bytes.push_back(c1);
        // This is 10zzzzzz
        char c2 =  static_cast<char>((codePoint >> 12) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c2);
        // This is 10yyyyyy
        char c3 =  static_cast<char>((codePoint >> 6) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c3);
        // This is 10xxxxxx
        char c4 =  static_cast<char>((codePoint) | (1 << 7)) & ~(1 << 6);
        utf8Bytes.push_back(c4);
    }
     
    return utf8Bytes;
}


