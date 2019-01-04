#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "des.hpp"
using namespace std;

int main()
{
    string input_str = "flag{this-is_4-simp1e-flag}";
    uint64_t temp64;
    uint64_t test = 0x363738393a3b3c3d;

    des ttetst;
    string ret1 = ttetst.Calc(input_str, test, 'e');
    cout << "CipherText: " << ret1 << endl;
    des deggg;
    string ret2 = deggg.Calc(ret1, test, 'd');
    cout << "PlainText: " << ret2 << endl;
}