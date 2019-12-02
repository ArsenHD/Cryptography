#include <cstring>
#include <fstream>

#include "RC4.h"

void read_input(vector <byte> &input) {
    ifstream in;
    in.open("../Test");

    string msg;
    string auxiliary_str;
    while (getline(in, auxiliary_str)) {
        msg += auxiliary_str;
        msg += "\n";
    }

    for (char c : msg) {
        input.push_back(c);
    }

    in.close();
}

int main() {
    cout << "Enter 40-bit key as 5 characters:" << endl;
    vector <byte> key;
    char keyChar;
    for (int i = 0; i < 5; i++) {
        cin >> keyChar;
        key.push_back(keyChar);
    }
    RC4 encryptor(key);

    vector <byte> message;
    read_input(message);

    vector <byte> cipher = encryptor.encrypt(message);
    vector <byte> decoded = encryptor.decrypt(cipher);

    ofstream out;
    out.open("../Output");

    out << "Initial message:" << endl;
    for (byte c : message) {
        out << c;
    }
    out << endl << endl;

    out << "Decoded message:" << endl;
    for (byte c : decoded) {
        out << c;
    }
    out << endl;

    out.close();
    
    return 0;
}