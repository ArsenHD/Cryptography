#include "MARS.h"

void strToArray(byte *&array, const string &str) {
    array = new byte[str.length() + 1];
    array[str.length()] = '\0';
    for (size_t i = 0; i < str.length(); i++) {
        array[i] = str[i];
    }
}

void read_input(byte *&message) {
    ifstream in;
    in.open("../Test");

    string msg;
    string auxiliary_str;
    while (getline(in, auxiliary_str)) {
        msg += auxiliary_str;
        msg += "\n";
    }

    strToArray(message, msg);

    in.close();
}

int getLength(const byte *str) {
    int counter = 0;
    while (str[counter] != '\0') {
        counter++;
    }
    return counter;
}

int main() {
    vector <uint32_t> key = { 29, 42, 52, 35 };

    byte *message = nullptr;
    read_input(message);

    auto *userKey = new byte[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            userKey[4 * i + j] = (key[i] >> static_cast<uint32_t>(8 * (3 - j))) & MARS::LAST;
        }
    }

    MARS encryptor(userKey, 16);

    string cph = encryptor.encrypt(message, getLength(message));

    byte *cipher;
    strToArray(cipher, cph);

    string decoded = encryptor.decrypt(cipher, getLength(cipher));

    ofstream out;
    out.open("../Output");

    out << "Initial message:" << endl;
    out << message << endl << endl << endl;

    out << "Decoded message:" << endl;
    out << decoded << endl;

    out.close();


    delete[] userKey;
    delete[] cipher;
    delete[] message;

    return 0;
}