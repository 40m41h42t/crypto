#include "des.hpp"
#include <iostream>
#include "global.hpp"
using namespace std;
des::des() {}
void des::encrypt(string PlainText, string key)
{
    this->PlainText = PlainText;
    this->key = key;
    isPlain = true;
    GenerateOutput();
}
void des::decrypt(string CypherText, string key)
{
    this->CypherText = CypherText;
    this->key = key;
    isPlain = false;
    GenerateOutput();
}
string des::GetPlainText(){
    return PlainText;
}
string des::GetCypherText(){
    return CypherText;
}
bitset<28> des::LoopLeftShift(bitset<28> k, int times)
{
    bitset<28> temp = k;
    for (int i = 27; i >= 0; --i)
    {
        if (i - times < 0)
            k[i] = temp[i - times + 28];
        else
            k[i] = temp[i - times];
    }
    return k;
}
void des::generate_key()
{
    bitset<64> FaKey = ToBitset(key);
    bitset<56> RealKey;
    // from 64 bit key to 56 bit key;
    for (int i = 0; i < 56; ++i)
        RealKey[55 - i] = FaKey[64 - PC_1[i]];
    bitset<28> LeftKey, RightKey;
    for (int round = 0; round < 16; ++round)
    {
        for (int i = 0; i < 28; ++i)
            RightKey[i] = RealKey[i];
        for (int i = 28; i < 56; ++i)
            LeftKey[i - 28] = RealKey[i];
        // LeftShift
        LeftKey = LoopLeftShift(LeftKey, shiftBits[round]);
        RightKey = LoopLeftShift(RightKey, shiftBits[round]);
        //
        for (int i = 0; i < 28; ++i)
            RealKey[i] = RightKey[i];
        for (int i = 28; i < 56; ++i)
            RealKey[i] = LeftKey[i - 28];
        // compress & replace
        for (int i = 0; i < 48; ++i)
        {
            SubKey[round][47 - i] = RealKey[56 - PC_2[i]];
        }
    }
}
void des::debug()
{
    string str = "12345678";
    bitset<64> temp = ToBitset(str);
    cout << temp << endl;
}
string des::ToString(bitset<64> btst)
{
    string ret;
    int tmp = 0;
    for (int i = 0; i < 64; ++i)
    {
        if (i % 8 == 0)
        {
            ret = ret + (char)tmp;
            tmp = 0;
        }
        tmp = tmp + btst[i] << (i % 8);
    }
    ret = ret + (char)tmp;
    return ret;
}
bitset<64> des::ToBitset(string str)
{
    bitset<64> ret;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            ret[i * 8 + j] = (str[i] >> j) & 1;
        }
    }
    return ret;
}
void des::GenerateOutput()
{
    generate_key();
    string *StrInput, *StrOutput;
    if (isPlain == true){
        StrInput = &PlainText;
        StrOutput = &CypherText;
    }
    else{
        StrInput = &CypherText;
        StrOutput = &PlainText;
    }
    long long count = 0;
    string block;
    while (true)
    {
        if (count + 8 >= StrInput->length())
            block = StrInput->substr(count, StrInput->length() - count);
        else
            block = StrInput->substr(count, 8);
        bitset<64> BlockBit = ToBitset(block);
        bitset<64> AfterBit = algorithm(BlockBit);
        StrOutput->append(ToString(AfterBit));
        count += 8;
        if (count > StrInput->length())
            break;
    }
}
bitset<64> des::algorithm(bitset<64> block)
{
    bitset<64> CypherBlock;
    // First replace IP
    bitset<64> AftBlock;
    for (int i = 0; i < 64; ++i)
        AftBlock[63 - i] = block[64 - IP[i]];
    // Second Get Left Half and Right Half
    bitset<32> left_block, right_block;
    for (int i = 0; i < 32; ++i)
        right_block[i] = AftBlock[i];
    for (int i = 32; i < 64; ++i)
        left_block[i - 32] = AftBlock[i];
    // Third Start 16 times iterations
    bitset<32> temp;
    for (int round = 0; round < 16; ++round)
    {
        temp = right_block;
        if (isPlain == true)
        {
            // encrypt
            right_block = left_block ^ F_Function(right_block, SubKey[round]);
        }
        else
        {
            // decrypt
            right_block = left_block ^ F_Function(right_block, SubKey[15 - round]);
        }
    }
    for (int i = 0; i < 32; ++i)
        CypherBlock[i] = right_block[i];
    for (int i = 32; i < 64; ++i)
        CypherBlock[i] = left_block[i - 32];
    AftBlock = CypherBlock;
    for (int i = 0; i < 64; ++i)
        CypherBlock[63 - i] = AftBlock[64 - IP_1[i]];
    return CypherBlock;
}
bitset<32> des::F_Function(bitset<32> block, bitset<48> roundkey)
{
    // 1. expand block from 32 to 48
    bitset<48> Expandblock;
    for (int i = 0; i < 48; ++i)
    {
        Expandblock[47 - i] = block[32 - E[i]];
    }
    // 2. xor expanded block and roundkey
    Expandblock = Expandblock ^ roundkey;
    // 3. found S Box
    bitset<32> AfterF;
    int temp = 0;
    for (int i = 0; i < 48; i += 6)
    {
        int row = Expandblock[47 - i] * 2 + Expandblock[47 - i - 5];
        int col = Expandblock[47 - i - 1] * 8 + Expandblock[47 - i - 2] * 4 +
                  Expandblock[47 - i - 3] * 2 + Expandblock[47 - i - 4];
        int num = S_BOX[i / 6][row][col];
        bitset<4> binary(num);
        AfterF[31 - temp] = binary[3];
        AfterF[31 - temp - 1] = binary[2];
        AfterF[31 - temp - 2] = binary[1];
        AfterF[31 - temp - 3] = binary[0];
        temp += 4;
    }
    // 4. P replace
    bitset<32> tmp = AfterF;
    for (int i = 0; i < 32; ++i)
    {
        AfterF[31 - i] = tmp[32 - P[i]];
    }
    return AfterF;
}