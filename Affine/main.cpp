#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "affine.hpp"
using namespace std;
int main()
{
    string text = "flag{this_is_a_simple_flag}";
    string cypher_text = "mkfd{qulz_lz_f_zlbakv_mkfd}";
    int k1 = 5, k2 = 17;
    while (true)
    {
        cout << "------ Affine Choice ------" << endl
             << "1: encrypt" << endl
             << "2: decrypt" << endl
             << "3: Word Anylasis" << endl
             << "other choice to exit" << endl
             << "---------------------------" << endl;

        int choice;
        cout << "your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            cout << "[+] Please input k1 k2" << endl;
            cin >> k1 >> k2;
            cout << "[√] k1: " << k1 << "\tk2: " << k2 << endl
                 << "[+] Please input filename" << endl;
            string filename;
            cin >> filename;
            ifstream enin(filename);
            if (!enin.is_open())
            {
                cout << "[x] error open file: " << filename << endl;
                break;
            }
            cout << "[√] filename: " << filename << endl;
            stringstream buffer;
            buffer << enin.rdbuf();
            string PlainText(buffer.str());
            enin.close();
            Affine Encrypt(PlainText, k1, k2, true);
            Encrypt.encrypt();
            ofstream enout("temp/encrypt.txt");
            enout << Encrypt.get_cypher_text();
            cout << "[√] ciphertext is written in file 'temp/encrypt.txt'" << endl;
            break;
        }
        case 2:
        {
            cout << "[+] Please input k1 k2" << endl;
            cin >> k1 >> k2;
            cout << "[√] k1: " << k1 << "\tk2: " << k2 << endl;
            cout << "[+] By default, ciphertext will be read in encrypt.txt" << endl;
            ifstream dein("temp/encrypt.txt");
            if (!dein.is_open())
            {
                cout << "[x] error open file: encrypt.txt" << endl;
                break;
            }
            stringstream buffer;
            buffer << dein.rdbuf();
            string CipherText(buffer.str());
            Affine Decrypt(CipherText, k1, k2, false);
            Decrypt.decrypt();
            cout << "[+] Please input saved-plaintext filename:" << endl;
            string saveFile;
            cin >> saveFile;
            ofstream saveout(saveFile);
            saveout << Decrypt.get_plain_text();
            cout << "[√] plaintext is written in file " << saveFile << endl;
            break;
        }
        case 3:
        {
            cout << "[+] Please input filename:" << endl;
            string anaFile;
            cin >> anaFile;
            ifstream anain(anaFile);
            if (!anain.is_open())
            {
                cout << "[x] error open file: " << anaFile << endl;
                break;
            }
            cout << "[√] filename: " << anaFile << endl;
            stringstream buffer;
            buffer << anain.rdbuf();
            string anaText(buffer.str());
            ifstream dein("temp/encrypt.txt");
            if (!dein.is_open())
            {
                cout << "[x] error open file: encrypt.txt" << endl;
                break;
            }
            stringstream buffer1;
            buffer1.clear();
            buffer1 << dein.rdbuf();
            string CipherText(buffer1.str());
            WordAnylasis Anylasis(CipherText, anaText);
            Anylasis.Anylasis();
            cout << "[+] Here is the table:" << endl;
            cout << Anylasis.GetTable() << endl;
            break;
        }
        default:
            exit(0);
        }
    }
}