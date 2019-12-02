#ifndef HW4_RC4_H
#define HW4_RC4_H

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef unsigned char byte;

class RC4 {
public:

    explicit RC4(vector <byte>);
    ~RC4() = default;

    vector <byte> encrypt(const vector <byte> &);
    vector <byte> decrypt(const vector <byte> &);

private:

    const vector <byte> key;

    vector <byte> S;

    void initS();

};


#endif //HW4_RC4_H
