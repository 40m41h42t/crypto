#ifndef RSA_HPP
#define RSA_HPP
#include <openssl/bn.h>
#include <string>
using namespace std;
class rsa
{
  private:
    // n = pq
    BIGNUM *n;
    // φ(n) = (p-1)(q-1)
    BIGNUM *phi_n;
    // Public Key, e
    BIGNUM *e;
    // Private Key, d
    BIGNUM *d;
    // Plain Text
    BIGNUM *m;
    // Cipher Text
    BIGNUM *c;
    // other num
    BIGNUM *one;
    BIGNUM *zero;

    BIGNUM *ExtGCD(BIGNUM *a, BIGNUM *b, BIGNUM *x, BIGNUM *y);
    void DebugOutPutBigNum(BIGNUM *a);
    void MODCalc(BIGNUM *result, BIGNUM *temp_x, BIGNUM *temp_r, BIGNUM *mod_n);
    string outPlainText;

  public:
    rsa(string PlainText);
    rsa();
    void Encrypt(string PlainText);
    void Decrypt(BIGNUM *CipherText, BIGNUM *PrivateKey, BIGNUM *outN);
    string Decrypt(string CipherText, string PrivateKey, string outN);
    string GetC();
    string GetM();
    string GetN();
    string GetE();
    string GetD();
    void debug();
};

#endif