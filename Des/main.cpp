#include <iostream>
#include <string>
#include <cstring>
#include "des.hpp"
using namespace std;
int main()
{
    string txt = "1";
    string key = "!qrzbing";
    // char word[] = "1234567";
    // bitset<64> tst(key);
    // cout << tst << endl;
    // for(int i=0;i<32;++i){
    //     cout<<tst[i];
    // }
    des DES;
    // DES.debug();
    DES.encrypt(txt, key);
    cout << "Plaintext: " << DES.GetPlainText() << " len: " << DES.GetPlainText().length() << endl
         << "Cyphertext: " << DES.GetCypherText() << " len: " << DES.GetCypherText().length() << endl;
    des DEP;
    DEP.decrypt(DES.GetCypherText(), key);
    cout << "Plaintext: " << DEP.GetPlainText() << " len: " << DEP.GetPlainText().length() << endl
         << "Cyphertext: " << DEP.GetCypherText() << " len: " << DEP.GetCypherText().length() << endl;
    // cout<<endl;
    return 0;
}