#include "A5.h"

A5::A5(const vector <byte> &initialKey, const vector <byte> &frameNumber) {
    this->initialKey = initialKey;
    this->frameNumber = frameNumber;

    key1.resize(114);
    key2.resize(114);

    setupRegisters();
}

void A5::setInitialKey(const vector<byte> &initialKey) {
    this->initialKey = initialKey;
}

void A5::setFrameNumber(const vector<byte> &frameNumber) {
    this->frameNumber = frameNumber;
}

void A5::generateKeys() {
    for (int i = 0; i < 114; i++) {
        shiftRegistersWithControl();
        key1[i] = R1[18] ^ R2[21] ^ R3[22];
    }

    for (int i = 0; i < 114; i++) {
        shiftRegistersWithControl();
        key2[i] = R1[18] ^ R2[21] ^ R3[22];
    }
}

void A5::setupRegisters() {
    R1.resize(19);
    R2.resize(22);
    R3.resize(23);

    for (int i = 0; i < 19; i++) {
        R1[i] = 0;
    }

    for (int i = 0; i < 22; i++) {
        R2[i] = 0;
    }

    for (int i = 0; i < 23; i++) {
        R3[i] = 0;
    }

    byte keyBit, frameBit;

    for (int i = 0; i < 64; i++) {
        shiftRegisters();

        keyBit = (initialKey[i / 8] >> (i & 7)) & 1;
        R1[0] ^= (keyBit & 1);
        R2[0] ^= (keyBit & 1);
        R3[0] ^= (keyBit & 1);
    }

    for (int i = 0; i < 22; i++) {
        shiftRegisters();

        frameBit = (frameNumber[i / 8] >> (i & 7)) & 1;
        R1[0] ^= (frameBit & 1);
        R2[0] ^= (frameBit & 1);
        R3[0] ^= (frameBit & 1);
    }

    for (int i = 0; i < 100; i++) {
        shiftRegistersWithControl();
    }
}

void A5::shiftRegisters() {
    byte temp = R1[18] ^ R1[17] ^ R1[16] ^ R1[13];
    for (int j = R1.size() - 1; j > 0; j--) {
        R1[j] = R1[j - 1];
    }
    R1[0] = temp;

    temp = R2[21] ^ R2[20];
    for (int j = R2.size() - 1; j > 0; j--) {
        R2[j] = R2[j - 1];
    }
    R2[0] = temp;

    temp = R3[22] ^ R3[21] ^ R3[20] ^ R3[7];
    for (int j = R3.size() - 1; j > 0; j--) {
        R3[j] = R3[j - 1];
    }
    R3[0] = temp;
}

byte A5::shiftRegistersWithControl() {
    byte res = R1[18] ^ R2[21] ^ R3[22];

    byte x = R1[8];
    byte y = R2[10];
    byte z = R3[10];

    byte F = (x & y) | (x & z) | (y & z);

    byte temp;

    if (x == F) {
        temp = R1[18] ^R1[17] ^R1[16] ^R1[13];
        for (int j = R1.size() - 1; j > 0; j--) {
            R1[j] = R1[j - 1];
        }
        R1[0] = temp;
    }

    if (y == F) {
        temp = R2[21] ^ R2[20];
        for (int j = R2.size() - 1; j > 0; j--) {
            R2[j] = R2[j - 1];
        }
        R2[0] = temp;
    }

    if (z == F) {
        temp = R3[22] ^ R3[21] ^ R3[20] ^ R3[7];
        for (int j = R3.size() - 1; j > 0; j--) {
            R3[j] = R3[j - 1];
        }
        R3[0] = temp;
    }

    return res;
}

void A5::printKeys() const {
    cout << "First key:" << endl;
    for (int i = 0; i < 114; i++) {
        cout << (key1[i] & 1);
        if (i % 8 == 7) {
            cout << " ";
        }
    }
    cout << endl;
    cout << endl;

    cout << "Second key:" << endl;
    for (int i = 0; i < 114; i++) {
        cout << (key2[i] & 1);
        if (i % 8 == 7) {
            cout << " ";
        }
    }
    cout << endl;
}
