//
// file Cipher.cpp
//

#include "Cipher.hpp"
#include <iostream>
#include <cmath>

Cipher::Cipher() {
    char c;

    //ASCII first printable character
    int first = 32; 

    int last = 127;
    int length = last - first;


    for (int i = 0; i < length; i++) {
        c = first + i;
        alphabet.push_back(c);
    }
}

// delete values of plaintext and ciphertext
void Cipher::reset() {
    plaintext.clear();
    ciphertext.clear();
}

// Converts a n-bit two's complement value of form:
// -2^(n-1) to 2^(n-1) - 1
// into a n-bit unsigned value of form:
// 0 to 2^n - 1
//
// A wrong input will return 2^n
//
// example for 8-bit:
// converts a number between -128 and 127
// into a number between 0 and 255
unsigned int Cipher::convertFromTwosComplement(int n, int bits) {
    int min = (int)(pow(2, bits - 1) * (-1));
    int max = (int)(pow(2, bits - 1) - 1);

    // wrong input n
    if (n < min || n > max )
        return (int)pow(2, bits);
    
    else
        return (n >= 0) ? n : (int)( n + pow(2, bits) );
}

// Converts a n-bit value of form:
// 0 to 2^n - 1
// into a n-bit two's complement value of form:
// -2^(n-1) to 2^(n-1) - 1
//
// A wrong input will return 2^n
//
// example for 8-bit:
// converts a number between 0 and 255
// into a number between -128 and 127
int Cipher::convertToTwosComplement(int n, int bits) {
    int min = 0;
    int max = (int)(pow(2, bits) - 1);

    // wrong input n
    if (n < min || n > max)
        return (int)pow(2, bits);
    
    else
        return (n < (int)(pow(2, bits - 1)) ? n : (int)( n - pow(2, bits)));
}

void Cipher::printAlphabet() {
    int nColumns = 4;
    
    cout << endl;

    for (int i = 0; i < (int)alphabet.size(); i++) {
        if ( (i % nColumns) == nColumns - 1) {
            printf("[%3d] %4d '%c'\n", i, alphabet[i], alphabet[i]);
        }
        else {
            printf("[%3d] %4d '%c'   ", i, alphabet[i], alphabet[i]);
        }
    }

    cout << endl;
    
    cout << "alphabet.size() = " << alphabet.size() << endl << endl;
}