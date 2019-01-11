#ifndef LFSR_HPP
#define LFSR_HPP
#define NUM 5
#include <string>
using namespace std;
class lfsr
{
  private:
    int status[NUM];
    string PlainText, CypherText;
    bool isPlain;
    int GetTop();

  public:
    lfsr(int *state);
    lfsr(string str, int *state, bool isPlain);
    void output_demo();
    void encrypt();
    void decrypt();
    string GetCypherText();
    string GetPlainText();
};

#endif