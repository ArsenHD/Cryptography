#ifndef HW3_MARS_H
#define HW3_MARS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <climits>

#include "Utils.h"

using namespace std;
using namespace Utils;

class MARS {
public:
    MARS(const byte *, int);
    ~MARS() = default;

    string encrypt(const byte *, int) const;
    string decrypt(const byte *, int) const;

    static const uint32_t BLOCK_SIZE = 16;

    static const uint32_t LAST = 255; // for getting low byte of a number via bitwise and
    
    static const uint32_t SECOND = 8; // for shifting second byte to the low byte
    static const uint32_t THIRD = 16; // for shifting THIRD byte to the low byte
    static const uint32_t HIGH = 24; // for shifting HIGH byte to the low byte

    static const uint32_t TWO_LAST_BITS = 3; // for obtaining two last bits of a number
    static const uint32_t FIVE_LAST_BITS = 31; // for obtaining five last bits of a number
    static const uint32_t NINE_LAST_BITS = 511; // for obtaining five last bits of a number

private:
    const byte *initialKey;
    int initialKeyLength;
    vector <uint32_t> key;
    vector <uint32_t> sBox1;
    vector <uint32_t> sBox2;
    vector <uint32_t> sBox;

    void initSBoxes();

    void initKey();
};


#endif //HW3_MARS_H
