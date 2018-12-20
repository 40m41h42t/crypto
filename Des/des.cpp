#include "des.hpp"
des::des() {}
void des::encrypt(string PlainText, string key)
{
    this->PlainText = PlainText;
    this->key = key;
    isPlain = true;
}
void des::decrypt(string CypherText, string key)
{
    this->CypherText = CypherText;
    this->key = key;
    isPlain = false;
}