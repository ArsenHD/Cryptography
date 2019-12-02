#include "RC4.h"

RC4::RC4(vector <byte> key) : key(std::move(key)) {}

vector <byte> RC4::encrypt(const vector <byte> &message) {
    initS();
    vector <byte> cipher(message.size());
    int i = 0, j = 0;
    int t;
    byte b;
    for (size_t k = 0; k < message.size(); k++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        t = (S[i] + S[j]) % 256;
        b = S[t];
        cipher[k] = message[k] ^ b;
    }

    return cipher;
}

vector <byte> RC4::decrypt(const vector <byte> &cipher) {
    return encrypt(cipher);
}

void RC4::initS() {
    S.clear();
    S.resize(256);
    generate(S.begin(), S.end(), [i = 0] () mutable { return i++; });

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key.size()]) % 256;
        swap(S[i], S[j]);
    }
}
