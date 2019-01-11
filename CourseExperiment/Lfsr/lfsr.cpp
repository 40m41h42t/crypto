#include "lfsr.hpp"
#include <cmath>
#include <iostream>
using namespace std;
#define MAXROUND ((int)pow(2, NUM))
lfsr::lfsr(int *state)
{
    // demo
    for (int i = 0; i < NUM; ++i)
    {
        status[i] = state[i];
    }
}
lfsr::lfsr(string str, int *state, bool isPlain)
{
    for (int i = 0; i < NUM; ++i)
    {
        status[i] = state[i];
    }
    this->isPlain = isPlain;
    if (isPlain)
    {
        PlainText = str;
    }
    else
    {
        CypherText = str;
    }
}
void lfsr::output_demo()
{
    // every sixteen status as one line
    for (int i = 0; i < MAXROUND * 2; ++i)
    {
        int temp = (status[0] + status[3]) % 2;
        int output = status[0];
        for (int j = 1; j < NUM; ++j)
        {
            status[j - 1] = status[j];
        }
        status[NUM - 1] = temp;
        cout << output << " ";
        if (i % 16 == 15)
        {
            cout << endl;
        }
    }
}
int lfsr::GetTop()
{
    int ret = status[0];
    for (int i = 1; i < NUM; ++i)
    {
        status[i - 1] = status[i];
    }
    status[NUM - 1] = (ret + status[2]) % 2;
    return ret;
}
void lfsr::encrypt()
{
    int j = 0;
    for (int i = 0; i < PlainText.length(); ++i)
    {
        char temp = PlainText.at(i);
        for (int k = 7; k >= 0; --k)
        {
            temp ^= (GetTop() << k);
        }
        CypherText = CypherText + (char)temp;
    }
}
void lfsr::decrypt()
{
    int j = 0;
    for (int i = 0; i < CypherText.length(); ++i)
    {
        char temp = CypherText.at(i);
        for (int k = 7; k >= 0; --k)
        {
            temp ^= (GetTop() << k);
        }
        PlainText = PlainText + (char)temp;
    }
}
string lfsr::GetCypherText()
{
    return CypherText;
}
string lfsr::GetPlainText()
{
    return PlainText;
}