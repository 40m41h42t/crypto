#include "des.hpp"
#include "global.hpp"
void des::GenerateKey()
{
    /* 28 bits */
    uint32_t C = 0, D = 0;
    /* 56 bits */
    uint64_t Bit56 = 0;
    /*  PC-1 permutation */
    //  Bits56[i] = Key64[PC-1[i]]
    for (int i = 0; i < 56; ++i)
    {
        Bit56 <<= 1;
        Bit56 |= (Key >> (64 - PC1[i])) & LB64_MASK;
    }

    C = (uint32_t)((Bit56 >> 28) & L28_MASK64);
    D = (uint32_t)(Bit56 & L28_MASK64);

    /* 16 rounds to generate SubKey */
    for (int rounds = 0; rounds < 16; ++rounds)
    {
        /*  In each round, C/D will LeftShift 
            iteration_shift[rounds] times, then merge to
            Bits56. After PC-2 permutation, Bit56 turns into
            SubKey[rounds]
        */

        /* LeftShift */
        for (int i = 0; i < iteration_shift[rounds]; ++i)
        {
            C = ((C << 1) & L28_MASK32) | ((C >> 27) & LB32_MASK);
            D = ((D << 1) & L28_MASK32) | ((D >> 27) & LB32_MASK);
        }

        /* Merged */
        Bit56 = (((uint64_t)C << 28) | (uint64_t)D) & L56_MASK;
        SubKey[rounds] = 0;

        /* PC-2 permutation */
        // SubKey[rounds][i] = Bit56[PC-2[i]]
        for (int i = 0; i < 48; ++i)
        {
            SubKey[rounds] <<= 1;
            SubKey[rounds] |= (Bit56 >> (56 - PC2[i])) & LB64_MASK;
        }
    }
}
uint32_t des::F_func(uint32_t R, int rounds)
{
    /* 8 bits */
    uint8_t row, col;

    /* 32 bits */
    uint32_t temp32 = 0;
    uint32_t ret32 = 0;

    /* 48 bits */
    uint64_t Bits48 = 0;

    /* E permutation */
    // Bits48[i] = R[E[i]]
    for (int i = 0; i < 48; ++i)
    {
        Bits48 <<= 1;
        Bits48 |= (uint64_t)((R >> (32 - E[i])) & LB32_MASK);
    }

    // Bits48 ^ Key[rounds]
    Bits48 = Bits48 ^ SubKey[rounds];

    /* S-box permutation */
    /*  In this Part, Bits48 will be departed to 8 parts: S1-S8,
        each part has 6 bits: b1b2b3b4b5b6. b1b6 will be the row,
        b2b3b4b5 will be the colomn. And number will be found in S-box.
    */
    for (int i = 0; i < 8; ++i)
    {
        // for 48 bit in uint64 is 0~12(departed to 6 parts),
        // each time, row=(Bits48>>(42-6*i))&(100001B)
        row = (uint8_t)(Bits48 >> (42 - 6 * i)) & 0x24;
        row = (row >> 5) | (row & 0x01);
        // colomn = (Bits48>>(42-6*i))&(1111B)
        col = (uint8_t)(Bits48 >> (42 - 6 * i + 1)) & 0xf;

        temp32 <<= 4;
        temp32 |= (uint32_t)(S[i][16 * row + col] & 0xf);
    }

    /* P permutation */
    // retlast[i] = retlast[P[i]]
    for (int i = 0; i < 32; ++i)
    {
        ret32 <<= 1;
        ret32 |= (temp32 >> (32 - P[i])) & LB32_MASK;
    }
    return ret32;
}
void des::Calc(uint64_t PlainText, uint64_t Key, char mode)
{
    /* 32 bits */
    uint32_t L = 0, R = 0;
    uint32_t temp32 = 0;

    /* 64 bits */
    uint64_t Bits64 = 0;

    /* initial */
    this->PlainText = PlainText;
    this->Key = Key;
    CypherText = 0;

    /* IP permutation */
    // Bits[i] = PlainText[IP[i]]
    for (int i = 0; i < 64; ++i)
    {
        Bits64 <<= 1;
        Bits64 |= (PlainText >> (64 - IP[i])) & LB64_MASK;
    }
    L = (uint32_t)(Bits64 >> 32) & L32_MASK64;
    R = (uint32_t)Bits64 & L32_MASK64;

    /* Generate SubKeys: Lazy condition */
    GenerateKey();

    /* 16 rounds calculate */
    for (int rounds = 0; rounds < 16; ++rounds)
    {
        // R_{i+1} = L_i ^ f(R_i, SubKey[i+1])
        // TODO: wait for optimization
        temp32 = R;
        if (mode == 'e')
            R = L ^ F_func(R, rounds);
        if (mode == 'd')
            R = L ^ F_func(R, 15 - rounds);
        L = temp32;
    }
    Bits64 = 0;
    Bits64 = ((uint64_t)R) << 32 | (uint64_t)L;

    /* PI permutation */
    // CypherText[i] = Bits64[PI[i]]
    for (int i = 0; i < 64; ++i)
    {
        CypherText <<= 1;
        CypherText |= (Bits64 >> (64 - PI[i])) & LB64_MASK;
    }
}
uint64_t des::GetCypherText()
{
    return CypherText;
}
uint64_t des::GetPlainText()
{
    return PlainText;
}