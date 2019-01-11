#include <iostream>
#include "dsa.hpp"
using namespace std;
int main()
{
    dsa test;
    test.Signature("Hi, qrzbing!");
    test.Check();
}