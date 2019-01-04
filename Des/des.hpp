#ifndef DES_HPP
#define DES_HPP
#include <cstdlib>
#include <cstdint>
#include <string>
using namespace std;
class des
{
  private:
    /* 64 bits */
    uint64_t Key;
    /* 48 bits */
    uint64_t SubKey[16];
    /* 4 bits */
    char mode;
    /* Calculation of the 16 SubKeys */
    string CypherText, PlainText;
    void GenerateKey();

    /* f()function, the core of DES */
    uint32_t F_func(uint32_t R, int rounds);

    uint64_t Algorithm(uint64_t TextBits64);
  public:
    /* Mainly calculate DES function */
    string Calc(string PlainText, uint64_t Key, char mode);

};

#endif