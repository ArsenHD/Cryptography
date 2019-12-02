#include <iostream>
#include <ctime>
#include <cstdlib>

#include "COMP128.h"
#include "A5.h"

int main() {
    srand(time(nullptr));

    vector <byte> ki(16);
    vector <byte> rand(16);

    for (int i = 0; i < 16; i++) {
        ki[i] = std::rand() % 256;
    }

    for (int i = 0; i < 16; i++) {
        rand[i] = std::rand() % 256;
    }

    COMP128 comp128(ki, rand);
    vector <byte> a3a8 = comp128.calculate();

    cout << "COMP128 result:" << endl;
    for (byte b : a3a8) {
        for (int i = 0; i < 8; i++) {
            cout << (Utils::getBit(b, i) & 1);
        }
        cout << " ";
    }
    cout << endl;
    cout << endl;

    vector <byte> frameNumber(3);

    for (size_t i = 0; i < frameNumber.size(); i++) {
        frameNumber[i] = std::rand() % 256;
    }

    for (int i = 22; i < 24; i++) {
        Utils::setBitAtPosition(frameNumber, 0, i);
    }

    vector <byte> key;
    for (size_t i = 4; i < a3a8.size(); i++) {
        key.push_back(a3a8[i]);
    }

    A5 a5 = A5(key, frameNumber);

    a5.generateKeys();
    a5.printKeys();

    return 0;
}