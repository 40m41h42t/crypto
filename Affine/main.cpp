#include <cstdio>
#include <iostream>
#include <string>
#include "affine.hpp"
using namespace std;
int main()
{
    string text = "flag{this_is_a_simple_flag}";
    string cypher_text = "mkfd{qulz_lz_f_zlbakv_mkfd}";
    int k1 = 5, k2 = 17;
    cout << "choice: " << endl
         << "1: encrypt" << endl
         << "2: decrypt" << endl
         << "3: Word Anylasis" << endl
         << "other choice to exit" << endl;
    int choice;
    cin >> choice;
    switch (choice)
    {
    case 1:
    {
        Affine Encrypt(text, k1, k2, true);
        Encrypt.encrypt();
        break;
    }
    case 2:
    {
        Affine Decrypt(cypher_text, k1, k2, false);
        Decrypt.decrypt();
        break;
    }
    case 3:
    {
        WordAnylasis Anylasis(cypher_text, text);
        Anylasis.Anylasis();
    }
    default:
        break;
    }
}