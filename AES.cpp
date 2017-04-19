//
// Created by 沛然 姚 on 2017/4/19.
//

#include "AES.h"

Byte *AES::encrypt(Byte *input, int inputLen, Byte *key, int keyLen) {
    return nullptr;
}

/*
 * multiplying two bytes in GF(2^8) using the AES irreducible polynominal.
 */
Byte AES::gMul(Byte a, Byte b) {
    Byte p = 0;
    Byte counter;
    Byte hi_bit_set;
    for (counter = 0; counter < 8; counter++) {
        if ((b & 1) != 0) {
            p ^= a;
        }
        hi_bit_set = (Byte) (a & 0x80);
        a <<= 1;
        if (hi_bit_set != 0) {
            a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */
        }
        b >>= 1;
    }
    return p;
}

Mat AES::mixColumns(Mat state) {
    Mat result;
    for (int y = 0; y < 4; y++) {
        result[0][y] = gMul(2, state[0][y]) ^ gMul(3, state[1][y]) ^ gMul(1, state[2][y]) ^ gMul(1, state[3][y]);
        result[1][y] = gMul(1, state[0][y]) ^ gMul(2, state[1][y]) ^ gMul(3, state[2][y]) ^ gMul(1, state[3][y]);
        result[2][y] = gMul(1, state[0][y]) ^ gMul(1, state[1][y]) ^ gMul(2, state[2][y]) ^ gMul(3, state[3][y]);
        result[3][y] = gMul(3, state[0][y]) ^ gMul(1, state[1][y]) ^ gMul(1, state[2][y]) ^ gMul(2, state[3][y]);
    }
    return result;
}
