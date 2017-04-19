#include <iostream>
#include <cstdio>
#include "AES.h"



int main() {

    std::cout << "Hello, World!" << std::endl;

    Mat state;
    state[0][0] = (Byte) 0x32;
    state[0][1] = (Byte) 0x88;
    state[0][2] = (Byte) 0x31;
    state[0][3] = (Byte) 0xe0;
    state[1][0] = (Byte) 0x43;
    state[1][1] = (Byte) 0x5a;
    state[1][2] = (Byte) 0x31;
    state[1][3] = (Byte) 0x37;
    state[2][0] = (Byte) 0xf6;
    state[2][1] = (Byte) 0x30;
    state[2][2] = (Byte) 0x98;
    state[2][3] = (Byte) 0x07;
    state[3][0] = (Byte) 0xa8;
    state[3][1] = (Byte) 0x8d;
    state[3][2] = (Byte) 0xa2;
    state[3][3] = (Byte) 0x34;

    Mat key;
    key[0][0] = (Byte) 0x2b;
    key[0][1] = (Byte) 0x28;
    key[0][2] = (Byte) 0xab;
    key[0][3] = (Byte) 0x09;
    key[1][0] = (Byte) 0x7e;
    key[1][1] = (Byte) 0xae;
    key[1][2] = (Byte) 0xf7;
    key[1][3] = (Byte) 0xcf;
    key[2][0] = (Byte) 0x15;
    key[2][1] = (Byte) 0xd2;
    key[2][2] = (Byte) 0x15;
    key[2][3] = (Byte) 0x4f;
    key[3][0] = (Byte) 0x16;
    key[3][1] = (Byte) 0xa6;
    key[3][2] = (Byte) 0x88;
    key[3][3] = (Byte) 0x3c;

//    AES::encrypt(state, key);
//    state.print();

    char plainText[] = {(char)0x32, 0x43, (char)0xf6, (char)0xa8,(char)0x88, 0x5a, 0x30, (char)0x8d, 0x31, 0x31, (char)0x98, (char)0xa2, (char)0xe0, 0x37, 0x07, 0x34};
    char keyword[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, (char)0xae, (char)0xd2, (char)0xa6, (char)0xab, (char)0xf7, 0x15, (char)0x88, 0x09, (char)0xcf, 0x4f, 0x3c};
    char* cipherText = AES::encrypt(plainText, 16, keyword, 16);
    for (int i = 0; i < 16; ++i) {
        printf("%hhx ", cipherText[i]);
    }
    return 0;
}