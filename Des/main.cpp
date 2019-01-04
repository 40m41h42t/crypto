#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "des.hpp"
using namespace std;

int main()
{
    string input_str = "flag{this-is_4-simp1e-flag}11100010101010";
    uint64_t temp64;
    // for (int i = 0; i < input_str.length(); i += 8)
    // {
    //     temp64 = *(uint64_t *)(input_str.c_str() + i);
    //     printf("%#llx\n", temp64);
    // }
    uint64_t test = 0x9474B8E8C73BCA7D;
    des ttt;
    ttt.Calc(test, test, 'e');
    printf("%#llx\n", ttt.GetPlainText());
    printf("%#llx\n", ttt.GetCypherText());
    des tt2;
    tt2.Calc(ttt.GetCypherText(), test, 'd');
    printf("%#llx\n", tt2.GetPlainText());
    printf("%#llx\n", tt2.GetCypherText());
}