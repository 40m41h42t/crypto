#include <iostream>
#include <string>
#include "lfsr.hpp"
using namespace std;

int main()
{
    int a[NUM] = {1, 1, 1, 1, 1};
    lfsr LFSR(a);
    LFSR.output_demo();
    cout<<endl;
    string str = "flag{this_is_a_simple_flag}";
    lfsr Encrypt(str, a, true);
    Encrypt.encrypt();
    cout << "Cyphertext:" << endl
         << Encrypt.GetCypherText() << endl;
    lfsr Decrypt(Encrypt.GetCypherText(), a, false);
    Decrypt.decrypt();
    cout << "PlainText:" << endl
         << Decrypt.GetPlainText() << endl;
    return 0;
}