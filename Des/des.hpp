#ifndef DES_HPP
#define DES_HPP
#include <string>
#include <bitset>
using namespace std;
class des
{
public:
  des();
  void Encrypt(string PlainText, string Key);
  void debug();

private:
  bool isPlainText;
  string PlainText, CypherText;
  string Key;
  bitset<48> SubKey[15];
  void GenerateKey();
  bitset<32> F_Function(bitset<32> Block, int round);
  bitset<28> LeftShift(bitset<28> bits, int times);
  bitset<64> String2Bitset(string str);
};
#endif