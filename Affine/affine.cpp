#include "affine.hpp"
#include <iostream>
#include <iomanip>
using namespace std;

Affine::Affine(string text, int k1, int k2, bool isPlain)
{
    this->text = text;
    this->isPlain = isPlain;
    key1 = k1;
    key2 = k2;
    init_text();
    test_key2(MOD);
}
int Affine::gcd(int num1, int num2)
{
    int temp = num2;
    while (num1 % num2 != 0)
    {
        temp = num1 % num2;
        num1 = num2;
        num2 = temp;
    }
    return temp;
}
void Affine::test_key2(int mod)
{
    if (gcd(key2, mod) != 1)
    {
        cout << "error, gcd(key2, 16)!=1" << endl;
        exit(0);
    }
}
int Affine::get_inverse(int k, int mod)
{
    for (int i = 0; i < mod; ++i)
    {
        if (i * k % mod == 1)
        {
            return i;
        }
    }
    return -1;
}
void Affine::init_text()
{
    string *temp_string;
    if (isPlain == true)
    {
        temp_string = &plain_text;
    }
    else
    {
        temp_string = &cypher_text;
    }
    for (int i = 0; i < text.length(); ++i)
    {
        char temp = text.at(i);
        if (temp >= 'A' && temp <= 'Z')
        {
            *temp_string = *temp_string + (char)(temp - 'A' + 'a');
        }
        else
        {
            *temp_string = *temp_string + temp;
        }
    }
}
void Affine::encrypt()
{
    if (isPlain == false)
    {
        cout << "error" << endl;
        exit(0);
    }
    cypher_text.clear();
    for (int i = 0; i < plain_text.length(); ++i)
    {
        if (plain_text.at(i) >= 'a' && plain_text.at(i) <= 'z')
        {
            // c = E_k(m) = (k_1 + k_2*m) mod 26
            char tempnum = (key1 + key2 * (plain_text.at(i) - 'a')) % MOD + 'a';
            cypher_text = cypher_text + tempnum;
        }
        else
        {
            cypher_text = cypher_text + plain_text.at(i);
        }
    }
}
void Affine::decrypt()
{
    if (isPlain == true)
    {
        cout << "error" << endl;
        exit(0);
    }
    plain_text.clear();
    char k2_inverse = (char)get_inverse(key2, MOD);
    for (int i = 0; i < cypher_text.length(); ++i)
    {
        if (cypher_text.at(i) >= 'a' && cypher_text.at(i) <= 'z')
        {
            // m = D_k(c) = k_{2}^{-1}(c - k_1) mod 26
            if (k2_inverse == -1)
            {
                cout << "error k" << endl;
                exit(0);
            }
            char temp = k2_inverse * (cypher_text.at(i) - 'a' - key1 + MOD) % MOD + 'a';
            plain_text = plain_text + temp;
        }
        else
        {
            plain_text = plain_text + cypher_text.at(i);
        }
    }
}
string Affine::get_cypher_text()
{
    return cypher_text;
}
string Affine::get_plain_text()
{
    return plain_text;
}

WordAnylasis::WordAnylasis(string ctext, string ptext)
{
    cypher_text = ctext;
    plain_text = ptext;
    for (int i = 0; i < MOD; ++i)
    {
        alphabet_plain[i] = alphabet_cypher[i] = 'a' + i;
        map_plain[alphabet_plain[i]] = 0;
        map_cypher[alphabet_cypher[i]] = 0;
    }
}
void WordAnylasis::PlainTextAnylasis()
{
    for (int i = 0; i < plain_text.length(); ++i)
    {
        if (plain_text.at(i) >= 'a' && plain_text.at(i) <= 'z')
        {
            ++map_plain[plain_text.at(i)];
        }
        else if (plain_text.at(i) >= 'A' && plain_text.at(i) <= 'Z')
        {
            ++map_plain[plain_text.at(i) - 'A' + 'a'];
        }
    }
}
void WordAnylasis::CypherTextAnylasis()
{
    for (int i = 0; i < cypher_text.length(); ++i)
    {
        if (cypher_text.at(i) >= 'a' && cypher_text.at(i) <= 'z')
        {
            ++map_cypher[cypher_text.at(i)];
        }
        if (cypher_text.at(i) >= 'A' && cypher_text.at(i) <= 'Z')
        {
            ++map_cypher[cypher_text.at(i) - 'A' + 'a'];
        }
    }
}
void WordAnylasis::Anylasis()
{
    stringstream anatable;
    anatable.clear();
    PlainTextAnylasis();
    CypherTextAnylasis();
    anatable << setw(7) << "|"
         << "-----plain text-----|-----cypher text----" << endl;
    anatable << "-char-|"
         << "--number-|-percent--|--number-|-percent--" << endl;
    for (int i = 0; i < MOD; ++i)
    {
        anatable << setw(4) << (char)('a' + i) << "  |"
                 << setw(9) << map_plain['a' + i] << "|"
                 << setw(10) << setprecision(3)
                 << (double)map_plain['a' + i] / plain_text.length() << "|"
                 << setw(9) << map_cypher['a' + i] << "|"
                 << setw(10) << setprecision(3)
                 << (double)map_cypher['a' + i] / cypher_text.length() << endl;
    }
    Table = anatable.str();
}
string WordAnylasis::GetTable()
{
    return Table;
}