#ifndef DSA_HPP
#define DSA_HPP
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <string>
using namespace std;
class dsa
{
private:
  BIGNUM *p, *q, *g;
  BIGNUM *x, *y;
  BIGNUM *one, *zero;
  BIGNUM *r, *k, *s;
  BIGNUM *m;
  BIGNUM *SHA1_m;
  string PlainText;
  void MODCalc(BIGNUM *result, BIGNUM *temp_x, BIGNUM *temp_r, BIGNUM *mod_n);
  BIGNUM *ExtGCD(BIGNUM *gcd_a, BIGNUM *gcd_b, BIGNUM *gcd_x, BIGNUM *gcd_y);
  void init();
  void output(BIGNUM *opt);

public:
  dsa();
  void Signature(string PlainText);
  void Check();
};

#endif