//
// Created by 沛然 姚 on 2017/4/19.
//

#include <cassert>
#include "AES.h"

#define ROTL8(x,shift) ((Byte) ((x) << (shift)) | ((x) >> (8 - (shift))))

Byte AES::SBox[16][16] = {0};
Byte AES::Rcon[16][4] = {
        {0x01, 0x00, 0x00, 0x00},
        {0x02, 0x00, 0x00, 0x00},
        {0x04, 0x00, 0x00, 0x00},
        {0x08, 0x00, 0x00, 0x00},
        {0x10, 0x00, 0x00, 0x00},
        {0x20, 0x00, 0x00, 0x00},
        {0x40, 0x00, 0x00, 0x00},
        {0x80, 0x00, 0x00, 0x00},
        {0x1b, 0x00, 0x00, 0x00},
        {0x36, 0x00, 0x00, 0x00},
        {0x6c, 0x00, 0x00, 0x00},
        {0xd8, 0x00, 0x00, 0x00},
        {0xab, 0x00, 0x00, 0x00},
        {0x4d, 0x00, 0x00, 0x00},
        {0x9a, 0x00, 0x00, 0x00},
        {0x2f, 0x00, 0x00, 0x00}
};

char* AES::encrypt(char *input, int inputLen, char *key, int keyLen) {
    Mat state;
    Mat keyMat;

    assert(inputLen == 16);

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            state[x][y] = (Byte) input[x + 4*y];
        }
    }
    if (keyLen == 16) { // AES-128
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                keyMat[i][j] = (Byte) key[i+4*j];
            }
        }
        encrypt(state, keyMat, 10);
    }

    char* result = new char[16];
    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            result[x + 4*y] = state[x][y];
        }
    }
    return result;
}

/*
 * multiplying two bytes in GF(2^8) using the AES irreducible polynomial.
 * implementation from Wikipedia
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

void AES::mixColumns(Mat& state) {
    std::vector<Byte> result(4);
    for (int y = 0; y < 4; y++) {
        result[0] = gMul(2, state[0][y]) ^ gMul(3, state[1][y]) ^ gMul(1, state[2][y]) ^ gMul(1, state[3][y]);
        result[1] = gMul(1, state[0][y]) ^ gMul(2, state[1][y]) ^ gMul(3, state[2][y]) ^ gMul(1, state[3][y]);
        result[2] = gMul(1, state[0][y]) ^ gMul(1, state[1][y]) ^ gMul(2, state[2][y]) ^ gMul(3, state[3][y]);
        result[3] = gMul(3, state[0][y]) ^ gMul(1, state[1][y]) ^ gMul(1, state[2][y]) ^ gMul(2, state[3][y]);
        for (int i = 0; i < 4; ++i) {
            state[i][y] = result[i];
        }
    }
}

/*
 * initialize AES S-Box. From Wikipedia.
 */
void AES::initSBox() {
    int p = 1;
    Byte q = 1;

    /* loop invariant: p * q == 1 in the Galois field */
    do {
        /* multiply p by 2 */
        p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

        /* divide q by 2 */
        q ^= q << 1;
        q ^= q << 2;
        q ^= q << 4;
        q ^= q & 0x80 ? 0x09 : 0;

        /* compute the affine transformation */
        Byte xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);
        SBox[(Byte)p/16][(Byte)p%16] = (Byte) (xformed ^ 0x63);
    } while (p != 1);

    /* 0 is a special case since it has no inverse */
    AES::SBox[0][0] = 0x63;
}

void AES::printSBox() {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            printf("%hhx ", SBox[i][j]);
        }
        printf("\n");
    }
}

void AES::subBytes(Mat& state) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = SBox[state[i][j] >> 4][state[i][j] & 0x0F];
        }
    }
}

void AES::shiftRows(Mat &state) {
    for (int i = 1; i < 4; ++i) {
        std::vector<Byte> row(4);
        for (int j = 0; j < 4; ++j) {
            int idx = j-i%4;
            idx = (idx<0) ? idx+4 : idx;
            row[idx] = state[i][j];
        }
        state[i] = row;
    }
}

void AES::addRoundKey(Mat &state, Mat &roundKey) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] ^= roundKey[i][j];
        }
    }
}

std::vector<Mat> AES::expandKey(Mat &key, int n) {
    std::vector<Mat> subkeys;
    subkeys.push_back(key);

    int rcon_idx = 0;
    while (subkeys.size() < n) {
        Mat subkey;
        // calculate first column
        // rotate word
        auto tmp = subkeys.back()[0][3];
        for (int i = 0; i < 3; ++i) {
            subkey[i][0] = subkeys.back()[i+1][3];
        }
        subkey[3][0] = tmp;

        // substitute
        for (int i = 0; i < 4; ++i) {
            auto b = subkey[i][0];
            subkey[i][0] = SBox[b >> 4][b & 0x0F];
        }

        // add rcon and w[i-4]
        for (int i = 0; i < 4; ++i) {
            subkey[i][0] = (subkey[i][0]) ^ (subkeys.back()[i][0]) ^ Rcon[rcon_idx][i];
        }

        // calculate 3 remaining columns
        for (int i = 1; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                subkey[j][i] = subkey[j][i-1] ^ subkeys.back()[j][i];
            }
//            subkey.print();
        }

        subkeys.push_back(subkey);
        rcon_idx ++;
    }
    return subkeys;
}

void AES::encrypt(Mat& input, Mat key, int round) {
    initSBox();
    auto roundKeys = expandKey(key, round+1);
    addRoundKey(input, key);
    for (int i = 1; i < round; ++i) {
        subBytes(input);
        shiftRows(input);
        mixColumns(input);
        addRoundKey(input, roundKeys[i]);
    }
    subBytes(input);
    shiftRows(input);
    addRoundKey(input, roundKeys[round]);
}


