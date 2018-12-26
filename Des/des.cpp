#include "des.hpp"
#include "global.hpp"
#include <iostream>
using namespace std;
des::des() {}
void des::debug()
{
    string str = "1234567890";
    bitset<64> test = String2Bitset(str);
    cout << test << endl;
    // GenerateKey();
}
bitset<64> des::String2Bitset(string str)
{
    // This function changes an 8-length-string
    // into a 64-size-bitset.
    bitset<64> ret;
    int count = 0;
    for (int i = 0; i < str.length(); ++i)
        for (int j = 0; j < 8; ++j)
        {
            ret[count] = (str.at(i) >> j) & 1;
            count++;
            if (count > 63)
                return ret;
        }
    return ret;
}
bitset<28> des::LeftShift(bitset<28> bits, int times)
{
    return bits << (28 - times) | bits >> times;
}
void des::GenerateKey()
{
    Key = "12345678";
    bitset<64> KeyBit64 = String2Bitset(Key);
    bitset<56> KeyBit56;
    for (int i = 0; i < 56; ++i)
        // K_56[i] = K_64[PC_1[i]]
        KeyBit56[55 - i] = KeyBit64[64 - PC_1[i]];
    cout << KeyBit64 << endl;
    cout << KeyBit56 << endl;
    bitset<28> C, D;
    for (int i = 0; i < 28; ++i)
    {
        D[i] = KeyBit56[i];
        C[i] = KeyBit56[i + 28];
    }
    for (int round = 0; round < 16; ++round)
    {
        C = LeftShift(C, ShiftBits[round]);
        D = LeftShift(D, ShiftBits[round]);
        for (int i = 0; i < 56; ++i)
        {
            KeyBit56[i] = D[i];
            KeyBit56[i + 28] = C[i];
        }
        for (int i = 0; i < 48; ++i)
            // SubKey[round][i] = KeyBit56[PC_2[i]]
            SubKey[round][47 - i] = KeyBit56[56 - PC_2[i]];
    }
}
void des::Encrypt(string PlainText, string Key)
{
    isPlainText = true;
    this->PlainText = PlainText;
    this->Key = Key;
}