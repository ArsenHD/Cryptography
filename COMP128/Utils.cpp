#include "Utils.h"

uint32_t Utils::toInt(const string &str) {
    unsigned int result = 0;

    for (size_t i = 0; i < 8 * str.size(); i++) {
        result += static_cast<uint32_t>(pow(2, 31 - i) * getBitAtPosition(str, i));
    }

    return result;
}

string Utils::toString(uint32_t num) {
    int counter = 0;

    string result(4, 0);

    while (counter < 32) {
        setBitAtPosition(result, num % 2, 31 - counter++);
        num /= 2;
    }

    return result;
}

uint32_t Utils::mod(int a, int b) {
    return ((b + (a % b)) % b);
}

const uint32_t Utils::shiftToLeft(uint32_t block, int shift) {
    uint32_t first;
    for (int i = 0; i < shift; i++) {
        first = block & static_cast<uint32_t>(pow(2, 31));
        block <<= ONE;
        block = block | ((first == static_cast<uint32_t>(pow(2, 31)))? ONE : ZERO);
    }
    return block;
}

const uint32_t Utils::shiftToRight(uint32_t block, int shift) {
    uint32_t last;
    for (int i = 0; i < shift; i++) {
        last = block & ONE;
        block >>= ONE;
        block = block | ((last == 1)? static_cast<uint32_t>(pow(2, 31)) : 0);
    }
    return block;
}

byte Utils::getBitAtPosition(const string &block, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    char ch = block[num];
    return getBit(ch, shift);
}

byte Utils::getBitAtPosition(const byte *block, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    char ch = block[num];
    return getBit(ch, shift);
}

byte Utils::getBitAtPosition(const vector <byte> &block, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    byte ch = block[num];
    return getBit(ch, shift);
}

void Utils::setBitAtPosition(string &block, byte ch, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    block[num] &= (byte) ((pow(2, 8) - 1) - pow(2, 7 - shift));
    if (ch == 1) {
        block[num] += (byte) pow(2, 7 - shift);
    }
}

void Utils::setBitAtPosition(byte *block, byte ch, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    block[num] &= (byte) ((pow(2, 8) - 1) - pow(2, 7 - shift));
    if (ch == 1) {
        block[num] += (byte) pow(2, 7 - shift);
    }
}

void Utils::setBitAtPosition(vector <byte> &block, byte ch, int pos) {
    int num = pos / 8;
    int shift = pos % 8;
    block[num] &= (byte) ((pow(2, 8) - 1) - pow(2, 7 - shift));
    if (ch == 1) {
        block[num] += (byte) pow(2, 7 - shift);
    }
}

byte Utils::getBit(byte ch, int pos) {
    return static_cast<byte>((ch >> (7 - pos)) & ONE);
}

uint32_t Utils::add(uint32_t a, uint32_t b) {
    return ((a + b) % static_cast<unsigned long>(pow(2, 32)));
}

uint32_t Utils::subtract(uint32_t a, uint32_t b) {
    return ((a - b) % static_cast<unsigned long>(pow(2, 32)));
}

uint32_t Utils::multiply(uint32_t a, uint32_t b) {
    return ((a * b) % static_cast<unsigned long>(pow(2, 32)));
}