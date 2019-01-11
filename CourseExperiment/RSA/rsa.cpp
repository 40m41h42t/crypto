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
    BN_dec2bn(&bb, "7");
    BN_dec2bn(&xx, "563");
    BN_dec2bn(&yy, "561");
    MODCalc(aa,bb,xx,yy);
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
    // DebugOutPutBigNum(m);
    // text length (bits)
    int len = HexStr.length() * 4;

    BN_CTX *ctx = BN_CTX_new();

    // generate prime p, q
    BIGNUM *p = BN_new();
    BIGNUM *q = BN_new();
    BN_generate_prime_ex(p, len, true, NULL, NULL, NULL);
    BN_generate_prime_ex(q, len, true, NULL, NULL, NULL);

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
    //
    cout << "------------------ My RSA Info -------------------" << endl
         << "PlainText: " << PlainText << endl
         << "p : " << BN_bn2hex(p) << endl
         << "q : " << BN_bn2hex(q) << endl
         << "n : " << BN_bn2hex(n) << endl
         << "Φn: " << BN_bn2hex(phi_n) << endl
         << "e:  " << BN_bn2hex(e) << endl
         << "d:  " << BN_bn2hex(d) << endl
         << "---------------- Encryption Info ----------------" << endl
         << "m:  " << BN_bn2hex(m) << endl
         << "c:  " << BN_bn2hex(c) << endl;
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
    cout << "---------------- Decryption Info ----------------" << endl
         << "c: " << BN_bn2hex(CipherText) << endl
         << "d: " << BN_bn2hex(PrivateKey) << endl
         << "n: " << BN_bn2hex(outN) << endl
         << "m: " << BN_bn2hex(PlainText) << endl
         << "PlainText: " << pplaintext << endl;
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
bool rsa::Miller_rabin(BIGNUM *InputNum)
{
    if (BN_is_zero(InputNum) == true)
    {
        cout << "By default I will generate a prime" << endl;
        BN_generate_prime_ex(InputNum, 256, false, NULL, NULL, NULL);
    }
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *two = BN_new();
    BN_dec2bn(&two, "2");
    if (BN_cmp(InputNum, two) == 0)
    {
        return true;
    }
    BIGNUM *temp = BN_new();
    BN_mod(temp, InputNum, two, ctx);
    if (BN_cmp(temp, zero) == 0)
    {
        return false;
    }
    BIGNUM *n_1 = BN_new();
    BN_sub(n_1, InputNum, one);
    BIGNUM *wr_k = BN_new();
    BIGNUM *wr_m = BN_new();
    BN_add(wr_m, n_1, zero);
    while (true)
    {
        BN_div(wr_m, temp, wr_m, two, ctx);
        if (BN_is_one(temp))
        {
            BN_mul(wr_m, wr_m, two, ctx);
            break;
        }
        BN_add(wr_k, wr_k, one);
    }
    BN_one(temp);
    BIGNUM *k_1 = BN_new();
    BN_sub(k_1, wr_k, one);
    BIGNUM *wr_b = BN_new();
    BIGNUM *wr_z = BN_new();
    cout << "---------- Miller-Rabin ----------" << endl
         << "n: " << BN_bn2hex(InputNum) << endl
         << "k: " << BN_bn2hex(wr_k) << endl
         << "m: " << BN_bn2hex(wr_m) << endl;
    for (int i = 0; i < 30; ++i)
    {
        BN_rand_range(wr_b, n_1);
        MODCalc(wr_z, wr_b, wr_m, InputNum);

        if (BN_is_one(wr_z) == true || BN_cmp(wr_z, n_1) == 0)
        {
            return true;
        }
        for (; BN_cmp(temp, k_1) == -1; BN_add(temp, temp, one))
        {
            if (BN_cmp(wr_z, k_1) == true)
            {
                return false;
            }
            MODCalc(wr_z, wr_z, two, InputNum);
            if (BN_cmp(wr_z, n_1) == 0)
            {
                return true;
            }
        }
    }
    return true;
}
bool rsa::Miller_rabin(string InputNum)
{
    BIGNUM *mr_in = BN_new();
    BN_dec2bn(&mr_in, InputNum.c_str());
    return Miller_rabin(mr_in);
}