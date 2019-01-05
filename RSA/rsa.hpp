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
    BIGNUM* ExtGCD(BIGNUM *a, BIGNUM *b, BIGNUM *x, BIGNUM*y);

  public:
    rsa(string PlainText);
    rsa();
    void Encrypt(string PlainText);
    string GetN();
};

#endif