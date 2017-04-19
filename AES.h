//
// Created by 沛然 姚 on 2017/4/19.
//

#ifndef AES_AES_H
#define AES_AES_H

#include <vector>
#include <cstdio>

typedef char Byte;

class Mat : public std::vector<std::vector<Byte> > {
public:
    Mat() : std::vector<std::vector<Byte> >(4, std::vector<Byte>(4, 0)) {
        // do nothing
    }

    void print() {
        for (auto& row : *this) {
            for (char byte: row) {
                printf("%hhx ", byte);
            }
            printf("\n");
        }
    }
};

class AES {
private:
    static Byte gMul(Byte a, Byte b);
public:
    static Byte* encrypt(Byte* input, int inputLen, Byte* key, int keyLen);
    static Mat mixColumns(Mat state);

};


#endif //AES_AES_H
