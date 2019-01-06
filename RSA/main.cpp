#include <iostream>
#include <sstream>
#include <fstream>
#include "rsa.hpp"
using namespace std;

void menu()
{
    cout << "1: encrypt" << endl
         << "2: decrypt" << endl
         << "other to quit" << endl;
}
int main()
{
    //
    int choice;
    while (true)
    {
        menu();
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            rsa en;
            ifstream infile("input.txt");
            if (!infile.is_open())
            {
                cout << "Err open input.txt" << endl;
                break;
            }
            stringstream buffer;
            buffer << infile.rdbuf();
            infile.close();
            string PlainText(buffer.str());
            // cout << PlainText << endl;
            // break;
            cout << "[+] Start Encrypt" << endl;
            en.Encrypt(PlainText);
            cout << "[+] CipherText will be saved in encrypt.txt" << endl;
            ofstream outfile("encrypt.txt");
            outfile << en.GetC();
            outfile.close();
            cout << "[+] N will be saved at rsa_n.txt" << endl;
            ofstream n_out("rsa_n.txt");
            n_out << en.GetN();
            n_out.close();
            cout << "[+] Public Key will be saved in rsa_pub.txt" << endl;
            ofstream pubout("rsa_pub.txt");
            pubout << en.GetE();
            pubout.close();
            cout << "[+] Private Key will be saved in rsa_pri.txt" << endl;
            ofstream priout("rsa_pri.txt");
            priout << en.GetD();
            priout.close();
            break;
        }
        case 2:
        {
            rsa rsa_de;
            cout << "[+] Cipher Text will be read in encrypt.txt" << endl;
            ifstream infile("encrypt.txt");
            if (!infile.is_open())
            {
                cout << "Err open encrypt.txt" << endl;
                break;
            }
            stringstream buffer;
            buffer << infile.rdbuf();
            infile.close();
            string CipherText(buffer.str());
            cout << "[+] read private key" << endl;
            ifstream N_in("rsa_n.txt");
            stringstream buffer_n;
            buffer_n << N_in.rdbuf();
            N_in.close();
            string N_str(buffer_n.str());
            ifstream D_in("rsa_pri.txt");
            stringstream buffer_d;
            buffer_d << D_in.rdbuf();
            D_in.close();
            string D_str(buffer_d.str());
            // cout << CipherText << endl
            //      << D_str << endl
            //      << N_str << endl;
            string PlainText = rsa_de.Decrypt(CipherText, D_str, N_str);
            cout << "[+] plain text will be saved in plain.txt" << endl;
            ofstream outPlain("plain.txt");
            outPlain << PlainText;
            outPlain.close();
            break;
        }
        default:
            exit(0);
        }
    }
    // ttst.debug();
}