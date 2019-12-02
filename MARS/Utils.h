//
// Created by User on 06.11.2019.
//

#ifndef HW3_UTILS_H
#define HW3_UTILS_H

#include <iostream>
#include <cmath>

using namespace std;

typedef unsigned char byte;

namespace Utils {

    static const uint32_t ZERO = 0;
    static const uint32_t ONE = 1;

    uint32_t toInt(const string &);

    string toString(uint32_t);

    uint32_t mod(int, int);

    const uint32_t shiftToLeft(uint32_t, int);

    const uint32_t shiftToRight(uint32_t, int);

    byte getBitAtPosition(const string &, int);

    byte getBitAtPosition(const byte *, int);

    void setBitAtPosition(string &, byte, int);

    void setBitAtPosition(byte *, byte, int);

    byte getBit(byte, int);

    uint32_t add(uint32_t, uint32_t);

    uint32_t subtract(uint32_t, uint32_t);

    uint32_t multiply(uint32_t, uint32_t);

}


#endif //HW3_UTILS_H
