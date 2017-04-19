#include <iostream>
#include "AES.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Mat state;
    state[0][0] = (Byte) 0xd4;
    state[0][1] = (Byte) 0xe0;
    state[0][2] = (Byte) 0xb8;
    state[0][3] = (Byte) 0x1e;
    state[1][0] = (Byte) 0xbf;
    state[1][1] = (Byte) 0xb4;
    state[1][2] = (Byte) 0x41;
    state[1][3] = (Byte) 0x27;
    state[2][0] = (Byte) 0x5d;
    state[2][1] = (Byte) 0x52;
    state[2][2] = (Byte) 0x11;
    state[2][3] = (Byte) 0x98;
    state[3][0] = (Byte) 0x30;
    state[3][1] = (Byte) 0xae;
    state[3][2] = (Byte) 0xf1;
    state[3][3] = (Byte) 0xe5;

    state = AES::mixColumns(state);
    state.print();
    return 0;
}