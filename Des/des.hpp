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
    uint64_t CypherText, PlainText, Key;
    /* 48 bits */
    uint64_t SubKey[16];

    /* Calculation of the 16 SubKeys */
    void GenerateKey();

    /* f()function, the core of DES */
    uint32_t F_func(uint32_t R, int rounds);

  public:
    /* Mainly calculate DES function */
    void Calc(uint64_t PlainText, uint64_t Key, char mode);

    /* other functions */
    uint64_t GetPlainText();
    uint64_t GetCypherText();
};

#endif