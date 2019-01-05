#include "rsa.hpp"
#include <cstring>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <cstdio>
rsa::rsa(string PlainText)
{
    string HexStr;
    string TempStr;
    stringstream TempStream;

    for (int i = 0; i < PlainText.length(); ++i)
    {
        TempStream << hex << int(PlainText.at(i));
        TempStream >> TempStr;
        TempStream.clear();
        HexStr += TempStr;
    }

    cout << HexStr << endl;
    m = BN_new();
    cout << BN_hex2bn(&m, HexStr.c_str()) << endl;
    char *tst = BN_bn2hex(m);
    string saha(tst);
    // printf("%s\n", tst);
    cout << saha << endl;
}
rsa::rsa()
{
    // init BIGNUM
    n = BN_new();
    phi_n = BN_new();
    e = BN_new();
    d = BN_new();
    m = BN_new();
    c = BN_new();
}
BIGNUM *rsa::ExtGCD(BIGNUM *a, BIGNUM *b, BIGNUM *x, BIGNUM *y)
{
    BN_CTX *ctx = BN_CTX_new();
    if (BN_is_zero(b))
    {
        BN_one(x);
        BN_zero(y);
        return a;
    }
    else
    {
        BIGNUM *zero = BN_new();
        BN_zero(zero);
        BIGNUM *amodb = BN_new();
        BN_mod(amodb, a, b, ctx);
        BIGNUM *r = BN_NEW();
        BN_add(r, ExtGCD(b, amodb, x, y), zero);
        BIGNUM *temp = BN_new();
        BN_add(temp, x, zero);
        BN_zero(x);
        BN_add(x, y, zero);
        BIGNUM *temp2 = BN_new();
        BN_div(temp2, NULL, a, b, ctx);
        BN_mul(temp2, temp2, y, ctx);
        BN_sub(temp2, temp, temp2);
        return temp;
    }
}
void rsa::Encrypt(string PlainText)
{
    string HexStr;
    string TempStr;
    stringstream TempStream;

    for (int i = 0; i < PlainText.length(); ++i)
    {
        TempStream << hex << int(PlainText.at(i));
        TempStream >> TempStr;
        TempStream.clear();
        HexStr += TempStr;
    }
    // string 2 BIGNUM m
    BN_hex2bn(&m, HexStr.c_str());
    // text length (bits)
    int len = HexStr.length() * 4;
    // generate prime p, q
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BN_generate_prime_ex(p, len * 3, true, NULL, NULL, NULL);
    BN_generate_prime_ex(q, len * 3, true, NULL, NULL, NULL);

    // n=p*q
    BN_CTX *ctx = BN_CTX_new();
    BN_mul(n, p, q, ctx);

    // Φ(n)=(p-1)(q-1)
    BIGNUM *one = BN_new();
    BN_dec2bn(&one, "1");
    BN_sub(p, p, one);
    BN_sub(q, q, one);
    BN_mul(phi_n, p, q, ctx);

    // Generate e
    BN_generate_prime_ex(e, len * 4, true, NULL, NULL, NULL);

    // (ed)≡1 modΦ(n)
    // Extended Euclidean algorithm
    BN_add(d,ExtGCD(x,d,r,e));
}