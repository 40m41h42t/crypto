#include "des.hpp"
#include "global.hpp"
#include <iostream>
using namespace std;
des::des() {}
void des::debug()
{
    // string str = "1234567890";
    // bitset<64> test = String2Bitset(str);
    // cout << test << endl;
    // GenerateKey();
    string test = "123";
    string temp = test.substr(0, 8);
    cout << temp << endl;
    cout << temp.length() << endl;
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
    GenerateKey();
    for (int i = 0; i < this->PlainText.length(); i += 8)
    {
        string temp = this->PlainText.substr(i, 8);
        // Get 64 bit block
        bitset<64> TempBit64 = String2Bitset(temp);
        // Start IP
        bitset<64> AfterIP;
        for (int j = 0; j < 64; ++j)
            // AfterIP[i] = PlainText[IP[i]]
            AfterIP[63 - j] = TempBit64[64 - IP[i]];
        // +++++ +++++
        //   R     L
        bitset<32> L, R;
        // Begin 16 times iterations
        for (int j = 0; j < 32; ++j)
        {
            L[j] = AfterIP[j];
            R[j] = AfterIP[j + 32];
        }
        // F function for 16 rounds
        bitset<32> NextL;
        for (int round = 0; round < 16; + round)
        {
            NextL = R;
            R = F_Function(R, round) ^ L;
            L = NextL;
        }
    }
}
bitset<32> des::F_Function(bitset<32> Block, int round)
{
    bitset<48> ExpandBlock;
    // 1. E function expand 32-bit block to 48-bit
    for (int i = 0; i < 48; ++i)
        // ExpandBlock[i] = Block[E[i]]
        ExpandBlock[47 - i] = Block[32 - E[i]];
    // 2. Do xor calc Block = Block ^ SubKey[round]
    ExpandBlock = ExpandBlock ^ SubKey[round];
    // ++++++ .... ++++++
    // 654321 .... 654321
    //   S8          S1
    // 3. S-Box Replace, input 6-bit-data, output 4-bit-data
    for (int i = 0, j = 0; i < 48; i += 6, j += 4)
    {
        // row = 16; col = 2345
        int row = ExpandBlock[i] * 2 + ExpandBlock[i + 5];
        int col = ExpandBlock[i + 1] * 8 + ExpandBlock[i + 2] * 4 + ExpandBlock[i + 3] * 2 + ExpandBlock[i + 4];
        // Get Data in SBox
        bitset<4> BitTemp(S_BOX[i / 6][row][col]);
        // BitTemp ++++
        // 1234 -> 4321
        Block[31 - j] = BitTemp[3];
        Block[31 - j - 1] = BitTemp[2];
        Block[31 - j - 2] = BitTemp[1];
        Block[31 - j - 3] = BitTemp[0];
    }
    // P replace function
    bitset<32> TempBlock = Block;
    for (int i = 0; i < 32; ++i)
        // Block[i] = TempBlock[P[i]]
        Block[31 - i] = TempBlock[32 - P[i]];
    return Block;
}