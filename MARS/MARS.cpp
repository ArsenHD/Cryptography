#include "MARS.h"

MARS::MARS(const byte *initialKey, int length) {
    this->initialKey = initialKey;
    this->initialKeyLength = length;

    initSBoxes();
    initKey();
}

string MARS::encrypt(const byte *message, int length) const {
    int blocksNumber = (length / BLOCK_SIZE) + ((length % BLOCK_SIZE == 0) ? 0 : 1);


    auto *msg = new byte[sizeof(byte) * blocksNumber * BLOCK_SIZE + 1];
    msg[sizeof(byte) * blocksNumber * BLOCK_SIZE] = '\0';


    if (length % 16 != 0) {
        auto *incompleteBlock = new byte[length % BLOCK_SIZE];
        memcpy(incompleteBlock, message + (BLOCK_SIZE * (blocksNumber - 1)), length % 16);


        auto *lastBlock = new byte[BLOCK_SIZE];
        for (int i = 0; i < 8 * (length % BLOCK_SIZE); i++) {
            setBitAtPosition(lastBlock, getBitAtPosition(incompleteBlock, i), i);
        }


        for (int i = 8 * (length % BLOCK_SIZE); i < 8 * BLOCK_SIZE; i++) {
            setBitAtPosition(lastBlock, 0, i);
        }


        memcpy(msg, message, BLOCK_SIZE * (blocksNumber - 1));
        memcpy(msg + BLOCK_SIZE * (blocksNumber - 1), lastBlock, BLOCK_SIZE);


        delete[] incompleteBlock;
        delete[] lastBlock;
    } else {
        memcpy(msg, message, length);
    }

    string cipher;

    for (int i = 0; i < blocksNumber; i++) {
        uint32_t a, b, c, d;

        byte blockA[4];
        byte blockB[4];
        byte blockC[4];
        byte blockD[4];
        for (int j = 16 * i, k = 0; j < 16 * i + 4; j++, k++) {
            blockA[k] = msg[j];
        }
        for (int j = 16 * i + 4, k = 0; j < 16 * i + 8; j++, k++) {
            blockB[k] = msg[j];
        }
        for (int j = 16 * i + 8, k = 0; j < 16 * i + 12; j++, k++) {
            blockC[k] = msg[j];
        }
        for (int j = 16 * i + 12, k = 0; j < 16 * i + 16; j++, k++) {
            blockD[k] = msg[j];
        }

        memcpy(&a, &blockA, sizeof(uint32_t));
        memcpy(&b, &blockB, sizeof(uint32_t));
        memcpy(&c, &blockC, sizeof(uint32_t));
        memcpy(&d, &blockD, sizeof(uint32_t));

        a = add(a, key[0]);
        b = add(b, key[1]);
        c = add(c, key[2]);
        d = add(d, key[3]);

        for (int j = 0; j < 8; j++) {
            b = b ^ sBox1[a & LAST];
            b = add(b, sBox2[(a >> SECOND) & LAST]);
            c = add(c, sBox1[(a >> THIRD) & LAST]);
            d = d ^ sBox2[(a >> HIGH) & LAST];

            a = shiftToRight(a, 24);
            if (j == 1 || j == 5) {
                a = add(a, b);
            } else if (j == 0 || j == 4) {
                a = add(a, d);
            }
            uint32_t t = a;
            a = b; b = c, c = d, d = t;
        }

        for (int j = 0; j < 16; j++) {
//            round function
            uint32_t M = add(a, key[2 * j + 4]);
            uint32_t R = multiply(shiftToLeft(a, 13), key[2 * j + 5]);
            uint32_t L = sBox[M & NINE_LAST_BITS];
            R = shiftToLeft(R, 5);
            M = shiftToLeft(M, R & FIVE_LAST_BITS);
            L = L ^ R;
            R = shiftToLeft(R, 5);
            L = L ^ R;
            L = shiftToLeft(L, R & FIVE_LAST_BITS);
//

            a = shiftToLeft(a, 13);
            c = add(c, M);

            if (j < 8) {
                b = add(b, L);
                d = d ^ R;
            } else {
                d = add(d, L);
                b = b ^ R;
            }

            uint32_t t = a;
            a = b; b = c; c = d; d = t;
        }

        for (int j = 0; j < 8; j++) {
            if (j == 3 || j == 7) {
                a = subtract(a, b);
            } else if (j == 2 || j == 6) {
                a = subtract(a, d);
            }
            b = b ^ sBox2[a & LAST];
            c = subtract(c, sBox1[(a >> HIGH) & LAST]);
            d = subtract(d, sBox2[(a >> THIRD) & LAST]);
            d = d ^ sBox1[(a >> SECOND) & LAST];

            a = shiftToLeft(a, 24);

            unsigned t = a;
            a = b; b = c; c = d; d = t;
        }

        a = subtract(a, key[36]);
        b = subtract(b, key[37]);
        c = subtract(c, key[38]);
        d = subtract(d, key[39]);

        auto *temp = new byte[16];
        memcpy(temp, &a, sizeof(uint32_t));
        memcpy(temp + sizeof(uint32_t), &b, sizeof(uint32_t));
        memcpy(temp + 2 * sizeof(uint32_t), &c, sizeof(uint32_t));
        memcpy(temp + 3 * sizeof(uint32_t), &d, sizeof(uint32_t));

        for (int j = 0; j < 16; j++) {
            cipher += temp[j];
        }

        delete[] temp;
    }

    delete[] msg;

    return cipher;
}

string MARS::decrypt(const byte *cipher, int length) const {
    int blocksNumber = (length / 16) + ((length % 16 == 0) ? 0 : 1);

    string message;

    for (int i = 0; i < blocksNumber; i++) {
        auto *block = new byte[16];
        memcpy(block, cipher + 16 * i, 16);

        uint32_t a, b, c, d;

        byte blockA[4];
        byte blockB[4];
        byte blockC[4];
        byte blockD[4];
        for (int j = 16 * i, k = 0; j < 16 * i + 4; j++, k++) {
            blockA[k] = cipher[j];
        }
        for (int j = 16 * i + 4, k = 0; j < 16 * i + 8; j++, k++) {
            blockB[k] = cipher[j];
        }
        for (int j = 16 * i + 8, k = 0; j < 16 * i + 12; j++, k++) {
            blockC[k] = cipher[j];
        }
        for (int j = 16 * i + 12, k = 0; j < 16 * i + 16; j++, k++) {
            blockD[k] = cipher[j];
        }

        memcpy(&a, &blockA, sizeof(uint32_t));
        memcpy(&b, &blockB, sizeof(uint32_t));
        memcpy(&c, &blockC, sizeof(uint32_t));
        memcpy(&d, &blockD, sizeof(uint32_t));

        a = add(a, key[36]);
        b = add(b, key[37]);
        c = add(c, key[38]);
        d = add(d, key[39]);

        for (int j = 7; j >= 0; j--) {
            uint32_t t = d;
            d = c; c = b; b = a; a = t;

            a = shiftToRight(a, 24);

            d = d ^ sBox1[(a >> SECOND) & LAST];
            d = add(d, sBox2[(a >> THIRD) & LAST]);
            c = add(c, sBox1[(a >> HIGH) & LAST]);
            b = b ^ sBox2[a & LAST];

            if (j == 2 || j == 6) {
                a = add(a, d);
            } else if (j == 3 || j == 7) {
                a = add(a, b);
            }
        }

        for (int j = 15; j >= 0; j--) {
            uint32_t t = d;
            d = c; c = b; b = a; a = t;

            a = shiftToRight(a, 13);

            uint32_t M = add(a, key[2 * j + 4]);
            uint32_t R = multiply(shiftToLeft(a, 13), key[2 * j + 5]);
            uint32_t L = sBox[M & NINE_LAST_BITS];
            R = shiftToLeft(R, 5);
            M = shiftToLeft(M, R & FIVE_LAST_BITS);
            L = L ^ R;
            R = shiftToLeft(R, 5);
            L = L ^ R;
            L = shiftToLeft(L, R & FIVE_LAST_BITS);

            c = subtract(c, M);

            if (j < 8) {
                b = subtract(b, L);
                d = d ^ R;
            } else {
                d = subtract(d, L);
                b = b ^ R;
            }
        }

        for (int j = 7; j >= 0; j--) {
            uint32_t t = d;
            d = c; c = b; b = a; a = t;

            if (j == 0 || j == 4) {
                a = subtract(a, d);
            } else if (j == 1 || j == 5) {
                a = subtract(a, b);
            }

            a = shiftToLeft(a, 24);

            d = d ^ sBox2[(a >> HIGH) & LAST];
            c = subtract(c, sBox1[(a >> THIRD) & LAST]);
            b = subtract(b, sBox2[(a >> SECOND) & LAST]);
            b = b ^ sBox1[a & LAST];
        }

        a = subtract(a, key[0]);
        b = subtract(b, key[1]);
        c = subtract(c, key[2]);
        d = subtract(d, key[3]);

        char a1 = a & LAST, a2 = (a >> SECOND) & LAST, a3 = (a >> THIRD) & LAST, a4 = (a >> HIGH) & LAST;
        char b1 = b & LAST, b2 = (b >> SECOND) & LAST, b3 = (b >> THIRD) & LAST, b4 = (b >> HIGH) & LAST;
        char c1 = c & LAST, c2 = (c >> SECOND) & LAST, c3 = (c >> THIRD) & LAST, c4 = (c >> HIGH) & LAST;
        char d1 = d & LAST, d2 = (d >> SECOND) & LAST, d3 = (d >> THIRD) & LAST, d4 = (d >> HIGH) & LAST;

        message += a1; message += a2; message += a3; message += a4;
        message += b1; message += b2; message += b3; message += b4;
        message += c1; message += c2; message += c3; message += c4;
        message += d1; message += d2; message += d3; message += d4;


//        auto *temp = new byte[16];
//        memcpy(temp, &a, sizeof(uint32_t));
//        memcpy(temp + sizeof(uint32_t), &b, sizeof(uint32_t));
//        memcpy(temp + 2 * sizeof(uint32_t), &c, sizeof(uint32_t));
//        memcpy(temp + 3 * sizeof(uint32_t), &d, sizeof(uint32_t));
//
//        for (int j = 0; j < 16; j++) {
//            cipher += temp[j];
//        }

//        delete[] temp;


        delete[] block;
    }

    return message;
}

void MARS::initSBoxes() {
    ifstream in1;
    in1.open("../res/sBox0");

    sBox1.resize(256);
    for (int i = 0; i < 256; i++) {
        in1 >> hex >> sBox1[i];
    }

    in1.close();

    ifstream in2;
    in2.open("../res/sBox1");

    sBox2.resize(256);
    for (int i = 0; i < 256; i++) {
        in2 >> hex >> sBox2[i];
    }

    in2.close();

    sBox.resize(512);
    for (int i = 0; i < 256; i++) {
        sBox[i] = sBox1[i];
    }
    for (int i = 0; i < 256; i++) {
        sBox[256 + i] = sBox2[i];
    }
}

void MARS::initKey() {
    key.resize(40);
    int n = initialKeyLength / 4;
    uint32_t T[15];

    memcpy(T, initialKey, initialKeyLength);

    T[n] = n;

    for (int i = n + 1; i < 15; i++) {
        T[i] = 0;
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 15; j++) {
            T[j] = T[j] ^ shiftToLeft(T[mod(j - 7, 15)] ^ T[mod(j - 2, 15)], 3) ^ (4 * j + i);
        }

        for (int k = 0; k < 4; k++) {
            for (int j = 0; j < 15; j++) {
                T[j] = shiftToLeft(add(T[j], sBox[T[mod(j - 1, 15)] & NINE_LAST_BITS]), 9);
            }
        }
        for (int j = 0; j < 10; j++) {
            key[10 * i + j] = T[(4 * j) % 15];
        }
    }

    uint32_t B[4] = { 0xa4a8d57b, 0x5b5d193b, 0xc8a8309b, 0x73f9a978 };
    for (int i = 5; i < 36; i += 2) {
        uint32_t j = key[i] & TWO_LAST_BITS;
        uint32_t w = key[i] | TWO_LAST_BITS;
        uint32_t M = 0;
        int zeroCounter = 0;
        int oneCounter = 0;
        for (int l = 0; l < 32; l++) {
            if (getBitAtPosition(toString(w), l) == 0) {
                zeroCounter++;
                oneCounter = 0;
            } else {
                oneCounter++;
                zeroCounter = 0;
            }

            if (zeroCounter == 10) {
                string temp = toString(M);
                for (int k = l - 9; k <= l; k++) {
                    setBitAtPosition(temp, 1, k);
                }
                M = toInt(temp);
            } else if (zeroCounter > 10) {
                string temp = toString(M);
                setBitAtPosition(temp, 1, l);
                M = toInt(temp);
            }

            if (oneCounter == 10) {
                string temp = toString(M);
                for (int k = l - 9; k <= l; k++) {
                    setBitAtPosition(temp, 1, k);
                }
                M = toInt(temp);
            } else if (oneCounter > 10) {
                string temp = toString(M);
                setBitAtPosition(temp, 1, l);
                M = toInt(temp);
            }
        }

        string mask = toString(M);
        for (int l = 0; l < 32; l++) {
            if ((l == 0 || getBitAtPosition(mask, l - 1) == 0) && (getBitAtPosition(mask, l) == 1)) {
                setBitAtPosition(mask, 0, l);
            } else if ((l == 31 || getBitAtPosition(mask, l + 1) == 0) && (getBitAtPosition(mask, l) == 1)) {
                setBitAtPosition(mask, 0, l);
            }
        }
        M = toInt(mask);
        uint32_t r = key[i - 1] & FIVE_LAST_BITS;
        uint32_t p = shiftToLeft(B[j], r);
        key[i] = w ^ (p & M);
    }
}