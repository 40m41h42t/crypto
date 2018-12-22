#ifndef DES_HPP
#define DES_HPP
#include <string>
#include <bitset>
using namespace std;

class des
{
  private:
    string PlainText;
    string CypherText;
    string key;
    bool isPlain;
    bitset<48> SubKey[16];
    void GenerateOutput();
    void generate_key();
    string ToString(bitset<64> btst);
    bitset<64> ToBitset(string str);
    bitset<64> algorithm(bitset<64> block);
    bitset<28> LoopLeftShift(bitset<28> k, int times);
    bitset<32> F_Function(bitset<32> block, bitset<48> roundkey);

  public:
    des();
    void encrypt(string PlainText, string key);
    void decrypt(string CypherText, string key);
    void debug();
    string GetPlainText();
    string GetCypherText();
};

#endif