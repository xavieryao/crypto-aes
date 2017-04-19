//
// Created by 沛然 姚 on 2017/4/19.
//

#ifndef AES_AES_H
#define AES_AES_H

#include <vector>
#include <cstdio>

typedef unsigned char Byte;

class Mat: public std::vector<std::vector<Byte> > {
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
        printf("\n");
    }
};



class AES {
private:
    static Byte gMul(Byte a, Byte b);

public:
    static Byte SBox[16][16];
    static Byte Rcon[16][4];

    static char* encrypt(char* input, int inputLen, char* key, int keyLen);
    static void encrypt(Mat& input, Mat key, int round);
    static void mixColumns(Mat& state);
    static void subBytes(Mat& state);
    static void shiftRows(Mat& state);
    static void addRoundKey(Mat& state, Mat& roundKey);
    static std::vector<Mat> expandKey(Mat& key, int n);
    static void initSBox();
    static void printSBox();
};


#endif //AES_AES_H
