#ifndef COMP128_A5_H
#define COMP128_A5_H

#include <iostream>
#include <vector>

#include "Utils.h"

class A5 {
public:
    A5(const vector <byte> &, const vector <byte> &);

    void setInitialKey(const vector <byte> &);
    void setFrameNumber(const vector <byte> &);

    void printKeys() const;

    void generateKeys();
private:

    vector <byte> initialKey;
    vector <byte> key1;
    vector <byte> key2;
    vector <byte> frameNumber;

    vector <byte> R1;
    vector <byte> R2;
    vector <byte> R3;

    void shiftRegisters();
    byte shiftRegistersWithControl();

    void setupRegisters();
};


#endif //COMP128_A5_H
