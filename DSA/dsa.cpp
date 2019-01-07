#include "dsa.hpp"
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;
void dsa::output(BIGNUM *opt)
{
    cout << BN_bn2hex(opt) << endl;
}
dsa::dsa()
{
    one = BN_new();
    BN_one(one);
    zero = BN_new();
    BN_zero(zero);
    p = BN_new();
    q = BN_new();
    g = BN_new();
    x = BN_new();
    y = BN_new();
    r = BN_new();
    k = BN_new();
    s = BN_new();
    m = BN_new();
    SHA1_m = BN_new();
}
BIGNUM *dsa::ExtGCD(BIGNUM *gcd_a, BIGNUM *gcd_b, BIGNUM *gcd_x, BIGNUM *gcd_y)
{
    BN_CTX *ctx = BN_CTX_new();
    if (BN_is_zero(gcd_b))
    {
        // *x=1,*y=0;
        BN_one(gcd_x);
        BN_zero(gcd_y);
        return gcd_a;
    }
    else
    {
        BIGNUM *amodb = BN_new();
        BN_mod(amodb, gcd_a, gcd_b, ctx);
        BIGNUM *gcd_r = BN_new();
        BN_add(gcd_r, ExtGCD(gcd_b, amodb, gcd_x, gcd_y), zero);
        BIGNUM *temp = BN_new();
        BN_add(temp, gcd_x, zero);
        BN_zero(gcd_x);
        BN_add(gcd_x, gcd_y, zero);
        BIGNUM *temp2 = BN_new();
        BN_div(temp2, NULL, gcd_a, gcd_b, ctx);
        BN_mul(temp2, temp2, gcd_y, ctx);
        BN_sub(gcd_y, temp, temp2);
        return temp;
    }
}
void dsa::MODCalc(BIGNUM *result, BIGNUM *temp_x, BIGNUM *temp_r, BIGNUM *mod_n)
{
    BIGNUM *a_new = BN_new();
    BN_add(a_new, temp_x, zero);
    BIGNUM *b_new = BN_new();
    BN_add(b_new, temp_r, zero);
    BN_one(result);
    BIGNUM *tempTwo = BN_new();
    BN_dec2bn(&tempTwo, "2");
    BIGNUM *TempNum = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    while (true)
    {
        if (BN_is_zero(b_new) == true)
        {
            return;
        }
        while (true)
        {
            BN_mod(TempNum, b_new, tempTwo, ctx);
            if (BN_is_zero(TempNum) == false)
                break;
            BN_div(b_new, NULL, b_new, tempTwo, ctx);
            BN_mul(a_new, a_new, a_new, ctx);
            BN_mod(a_new, a_new, mod_n, ctx);
        }
        BN_sub(b_new, b_new, one);
        BN_mul(result, result, a_new, ctx);
        BN_mod(result, result, mod_n, ctx);
    }
}
void dsa::init()
{
    // init
    BN_CTX *ctx = BN_CTX_new();
    // q 160bit
    BN_generate_prime_ex(q, 160, true, NULL, NULL, NULL);
    // q≡1 mod p, p,1024bit
    BN_generate_prime_ex(p, 1024, false, q, NULL, NULL);
    BIGNUM *h = BN_new();
    // h=rand(1,p-1)
    BN_rand_range(h, p);

    // g=h^((p-1)/q)mod p
    BIGNUM *temp = BN_new();
    BN_sub(temp, p, one);
    BN_div(temp, NULL, temp, q, ctx);
    MODCalc(g, h, temp, p);
    BN_free(h);
    BN_free(temp);
    // x = rand(0,q)

    BN_rand_range(x, q);
    // y=g^x mod p
    MODCalc(y, g, x, p);
    /* Public Key: (p,q,g,y) */
    /* Private Key: (x) */
    cout << "--------- My DSA info ----------" << endl;
    cout << "[+] p: " << BN_bn2hex(p) << endl;
    cout << "[+] q: " << BN_bn2hex(q) << endl;
    cout << "[+] g: " << BN_bn2hex(g) << endl;
    cout << "[+] y: " << BN_bn2hex(y) << endl;
    cout << "[+] x: " << BN_bn2hex(x) << endl;
}
void dsa::Signature(string PlainText)
{
    // init
    init();
    // cout << "init" << endl;
    BN_CTX *ctx = BN_CTX_new();
    this->PlainText = PlainText;
    string HexStr, TempStr;
    stringstream TempStream;
    for (int i = 0; i < PlainText.length(); ++i)
    {
        TempStream << hex << int(PlainText.at(i));
        TempStream >> TempStr;
        TempStream.clear();
        HexStr += TempStr;
    }
    BN_hex2bn(&m, HexStr.c_str());
    unsigned char hash1[20];
    SHA_CTX sha;
    SHA1_Init(&sha);
    SHA1_Update(&sha, HexStr.c_str(), HexStr.length());
    SHA1_Final(hash1, &sha);
    string HashStr((char *)hash1);
    string HashHex;
    for (int i = 0; i < HashStr.length(); ++i)
    {
        TempStream << hex << (int)(unsigned char)(HashStr.at(i));
        TempStream >> TempStr;
        TempStream.clear();
        HashHex += TempStr;
    }
    // cout << HashHex << endl;
    BN_hex2bn(&SHA1_m, HashHex.c_str());
    /* k = rand(0,q) */
    BN_rand_range(k, q);
    /* r=(g^k mod p)mod q */
    MODCalc(r, g, k, p);
    BN_mod(r, r, q, ctx);
    /* s=(k^(-1)(H(m)+xr))mod q */
    // FIXME: SHA1 Hash by myself
    BIGNUM *k_1 = BN_new();
    BIGNUM *temp = BN_new();
    ExtGCD(k, q, k_1, temp);
    if (BN_cmp(zero, k_1) == 1)
        BN_add(k_1, k_1, q);
    BN_mul(s, x, r, ctx);
    BN_add(s, s, SHA1_m);
    BN_mul(s, s, k_1, ctx);
    BN_mod(s, s, q, ctx);
    BN_free(k_1);
    BN_free(temp);
    cout << "---------- Message info ----------" << endl;
    cout << "[+] Message: " << PlainText << endl;
    cout << "[+] Digest:  " << BN_bn2hex(SHA1_m) << endl;
    cout << "[+] r:       " << BN_bn2hex(r) << endl;
    cout << "[+] s:       " << BN_bn2hex(s) << endl;

}
void dsa::Check()
{
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *w = BN_new();
    /* w=s^(-1)mod q */

    BIGNUM *temp = BN_new();
    ExtGCD(s, q, w, temp);
    if (BN_cmp(zero, w) == 1)
        BN_add(w, w, q);

    output(w);

    /*
    u_1=H(m)w mod q
    u_2=rw mod q
    */
    BIGNUM *u_1 = BN_new();
    BIGNUM *u_2 = BN_new();
    // TODO: change m to H_m
    BN_mul(u_1, SHA1_m, w, ctx);
    BN_mod(u_1, u_1, q, ctx);
    BN_mul(u_2, r, w, ctx);
    BN_mod(u_2, u_2, q, ctx);

    /* v=(g^(u1)y^(u2)mod p)mod q */
    BIGNUM *v = BN_new();
    MODCalc(temp, g, u_1, p);
    MODCalc(v, y, u_2, p);
    BN_mul(v, v, temp, ctx);
    BN_mod(v, v, p, ctx);
    BN_mod(v, v, q, ctx);
    cout << "---------- Verification info ----------" << endl;
    cout<<"[+] w:  "<<BN_bn2hex(w)<<endl;
    cout<<"[+] u1: "<<BN_bn2hex(u_1)<<endl;
    cout<<"[+] u2: "<<BN_bn2hex(u_2)<<endl;
    cout<<"[+] v:  "<<BN_bn2hex(v)<<endl;

    if (BN_cmp(v, r) == 0)
    {
        cout << "[+][Verified] OK" << endl;
    }
    else
    {
        cout << "[-][Verified] fail" << endl;
    }
}