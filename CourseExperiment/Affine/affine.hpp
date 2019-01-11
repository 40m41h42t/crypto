#ifndef AFFINE_H
#define AFFINE_H
#define MOD 26
#include <string>
#include <sstream>
#include <map>
using namespace std;

class Affine
{
  public:
    Affine(string text, int k1, int k2, bool isPlain);
    void encrypt();
    void decrypt();
    string get_cypher_text();
    string get_plain_text();

  private:
    string text;
    bool isPlain;
    int key1, key2;
    string cypher_text;
    string plain_text;
    void init_text();
    void test_key2(int mod);
    int gcd(int num1, int num2);
    int get_inverse(int k, int mod);
};
class WordAnylasis
{
  public:
    WordAnylasis(string ctext, string ptext);
    void PlainTextAnylasis();
    void CypherTextAnylasis();
    void Anylasis();
    string GetTable();
  private:
    string cypher_text;
    string plain_text;
    string Table;
    char alphabet_plain[MOD], alphabet_cypher[MOD];
    map<char, int> map_plain, map_cypher;
};
#endif