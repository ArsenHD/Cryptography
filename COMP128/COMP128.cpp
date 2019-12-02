//
// Created by User on 27.11.2019.
//

#include "COMP128.h"

COMP128::COMP128(const vector<byte> &ki, const vector<byte> &rand) {
    this->ki = ki;
    this->rand = rand;
}

vector<byte> COMP128::calculate() const {
    vector <byte> x(32);
    vector <byte> bit(128);

    int nextBit;

    for (int i = 16; i < 32; i++) {
        x[i] = rand[i - 16];
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
            x[j] = ki[j];
        }

        for (int j = 0;  j < 5; j++) {
            for (int k = 0; k < (1 << j); k++) {
                for (int l = 0; l < (1 << (4 - j)); l++) {
                    int m = 1 + k * (1 << (5 - j));
                    int n = m + (1 << (4 - j));
                    int y = (x[m] + 2 * x[n]) % (1 << (9 - j));
                    int z = (2 * x[m] + x[n]) % (1 << (9 - j));
                    x[m] = table[j][y];
                    x[n] = table[j][z];
                }
            }
        }

        for (int j = 0; j < 32; j++) {
            for (int k = 0; k < 4; k++) {
                bit[4 * j + k] = (x[j] >> (3 - k)) & 1;
            }
        }

        if (i < 7) {
            for (int j = 0; j < 16; j++) {
                x[j + 16] = 0;
                for (int k = 0; k < 8; k++) {
                    nextBit = ((8 * j + k) * 17) % 128;
                    x[j + 16] |= bit[nextBit] << (7 - k);
                }
            }
        }
    }

    vector <byte> res(12);

    for (int i = 0; i < 4; i++) {
        res[i] = (x[2 * i] << 4) | x[2 * i + 1];
    }

    for (int i = 0; i < 6; i++) {
        res[4 + i] = (x[2 * i + 18] << 6) | (x[2 * i + 19] << 2) | (x[2 * i + 20] >> 2);
    }

    res[10] = (x[30] << 6) | (x[31] << 2);
    res[11] = 0;
    return res;
}

void COMP128::setKi(const vector<byte> &ki) {
    this->ki = ki;
}

void COMP128::setRand(const vector<byte> &rand) {
    this->rand = rand;
}
