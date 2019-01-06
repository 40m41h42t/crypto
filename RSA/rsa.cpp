#include "rsa.hpp"
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <cstdio>
void rsa::DebugOutPutBigNum(BIGNUM *a)
{
    cout << "[+] " << BN_bn2hex(a) << endl;
}
void rsa::debug()
{
    BIGNUM *aa = BN_new();
    BIGNUM *bb = BN_new();
    BIGNUM *xx = BN_new();
    BIGNUM *yy = BN_new();
    BN_dec2bn(&aa, "3");
    BN_dec2bn(&bb, "220");
    ExtGCD(aa, bb, xx, yy);
    if (BN_cmp(zero, xx) == 1)
    {
        BN_add(xx, xx, bb);
    }
    DebugOutPutBigNum(xx);
    DebugOutPutBigNum(yy);
}
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
    one = BN_new();
    BN_one(one);
    zero = BN_new();
    BN_zero(zero);
}
BIGNUM *rsa::ExtGCD(BIGNUM *a, BIGNUM *b, BIGNUM *x, BIGNUM *y)
{
    BN_CTX *ctx = BN_CTX_new();
    if (BN_is_zero(b))
    {
        // *x=1,*y=0;
        BN_one(x);
        BN_zero(y);
        return a;
    }
    else
    {
        // r = gcdEX(b,a%b,x,y);
        // BIGNUM *zero = BN_new();
        // BN_zero(zero);
        BIGNUM *amodb = BN_new();
        BN_mod(amodb, a, b, ctx);
        BIGNUM *r = BN_new();
        BN_add(r, ExtGCD(b, amodb, x, y), zero);
        // t = *x
        BIGNUM *temp = BN_new();
        BN_add(temp, x, zero);
        BN_zero(x);
        // *x = *y
        BN_add(x, y, zero);
        // *y = t-a/b *(*y)
        BIGNUM *temp2 = BN_new();
        BN_div(temp2, NULL, a, b, ctx);
        BN_mul(temp2, temp2, y, ctx);
        BN_sub(y, temp, temp2);
        return temp;
    }
}
void rsa::MODCalc(BIGNUM *result, BIGNUM *temp_x, BIGNUM *temp_r, BIGNUM *mod_n)
{
    BIGNUM *a_new = BN_new();
    BN_add(a_new, temp_x, zero);
    BIGNUM *b_new = BN_new();
    BN_add(b_new, temp_r, zero);
    // c=1;
    // BIGNUM *c_ret = BN_new();
    BN_one(result);
    BIGNUM *tempTwo = BN_new();
    BN_dec2bn(&tempTwo, "2");
    BIGNUM *TempNum = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    while (true)
    {
        // if b==0:
        //      return c
        if (BN_is_zero(b_new) == true)
        {
            // BN_add(result, c_ret, one);
            return;
        }
        while (true)
        {
            BN_mod(TempNum, b_new, tempTwo, ctx);
            // b%2!=0
            if (BN_is_zero(TempNum) == false)
                break;
            BN_div(b_new, NULL, b_new, tempTwo, ctx);
            // a = (a*a)%n
            BN_mul(a_new, a_new, a_new, ctx);
            BN_mod(a_new, a_new, mod_n, ctx);
        }
        BN_sub(b_new, b_new, one);
        BN_mul(result, result, a_new, ctx);
        BN_mod(result, result, mod_n, ctx);
    }
}
void rsa::Encrypt(string PlainText)
{
    string HexStr;
    string TempStr;
    stringstream TempStream;
    // from string to BIGNUM
    for (int i = 0; i < PlainText.length(); ++i)
    {
        TempStream << hex << int(PlainText.at(i));
        TempStream >> TempStr;
        TempStream.clear();
        HexStr += TempStr;
    }
    BN_hex2bn(&m, HexStr.c_str());
    DebugOutPutBigNum(m);
    // text length (bits)
    int len = HexStr.length() * 4;

    BN_CTX *ctx = BN_CTX_new();

    // generate prime p, q
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BN_generate_prime_ex(p, len * 2, true, NULL, NULL, NULL);
    BN_generate_prime_ex(q, len * 2, true, NULL, NULL, NULL);

    // n=p*q
    BN_mul(n, p, q, ctx);

    // Φ(n)=(p-1)(q-1)
    // BIGNUM *one = BN_new();
    // BN_dec2bn(&one, "1");
    BN_sub(p, p, one);
    BN_sub(q, q, one);
    BN_mul(phi_n, p, q, ctx);
    // Generate e
    // set default e 65537
    // BN_generate_prime_ex(e, len * 4, true, NULL, NULL, NULL);
    BN_dec2bn(&e, "65537");
    // (ed)≡1 modΦ(n)
    // Extended Euclidean algorithm
    BIGNUM *x = BN_new();
    ExtGCD(e, phi_n, d, x);
    if (BN_cmp(zero, d) == 1)
    {
        BN_add(d, d, phi_n);
    }

    // debug if ed≡1(mod phi_n)
    BN_mul(x, e, d, ctx);
    BN_mod(x, x, phi_n, ctx);

    // debug if gcd(e,Φn)=1
    // BN_gcd(x, e, phi_n, ctx);
    // DebugOutPutBigNum(x);

    // now we get m, e, n, d.
    // we can calculate c,
    // in all datas,
    // n, e, c are public
    // d, m are private

    // c = m^e mod n
    MODCalc(c, m, e, n);
    // if (BN_cmp(zero, c) == 1)
    // {
    //     BN_add(c, c, n);
    // }
}
string rsa::Decrypt(string CipherText, string PrivateKey, string outN)
{
    // string temp = "123";
    BN_hex2bn(&c, CipherText.c_str());
    BN_hex2bn(&d, PrivateKey.c_str());
    BN_hex2bn(&n, outN.c_str());
    Decrypt(c, d, n);
    // return temp;
    return outPlainText;
}
void rsa::Decrypt(BIGNUM *CipherText, BIGNUM *PrivateKey, BIGNUM *outN)
{
    BIGNUM *PlainText = BN_new();
    MODCalc(PlainText, CipherText, PrivateKey, outN);
    string tempstr = string(BN_bn2hex(PlainText));
    cout << tempstr << endl;
    char tempch[3];
    tempch[2] = '\0';
    string pplaintext;
    for (int i = 0; i < tempstr.length(); i += 2)
    {
        memcpy(tempch, tempstr.c_str() + i, 2);
        // cout << atoi(tempch)<<" ";
        pplaintext = pplaintext + (char)strtol(tempch, NULL, 16);
    }
    outPlainText = pplaintext;
    // DebugOutPutBigNum(PlainText);
}
string rsa::GetC()
{
    return string(BN_bn2hex(c));
}
string rsa::GetM()
{
    return string(BN_bn2hex(m));
}
string rsa::GetN()
{
    return string(BN_bn2hex(n));
}
string rsa::GetE()
{
    return string(BN_bn2hex(e));
}
string rsa::GetD()
{
    return string(BN_bn2hex(d));
}