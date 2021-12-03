//
// file Caesar.cpp
//

#include "Caesar.hpp"

// check if index exists
int Caesar::checkIndex(int n) {
    int size = (int)alphabet.size();

    if (n < 0) {
        n += size;
    }
    if (n > size) {
        n -= size;
    }

    return n;
}

Caesar::Caesar() {
    for (int i = 128; i < 255; i++) {
        // avoid character SHY (soft hyphen)
        if (i != 240) {
            alphabet.push_back(i);
        }
    }
}

// delete values of variables plaintext, ciphertext and key
void Caesar::reset() {
    Cipher::reset();
    key = 0;
}

void Caesar::encrypt() {
    char c;
    int index;

    for (int i = 0; i < (int)plaintext.size(); i++) {
        c = plaintext[i];

        for (int j = 0; j < (int)alphabet.size(); j++) {
            if ( c == alphabet[j] ) {
                index = (j+key) % (int)alphabet.size();

                index = checkIndex(index);
                
                ciphertext.push_back(alphabet[index]);
                
                break;
            }
        }
    }
}

void Caesar::decrypt() {
    char c;
    int index;

    for (int i = 0; i < (int)ciphertext.size(); i++) {        
        c = ciphertext[i];

        for (int j = 0; j < (int)alphabet.size(); j++) {
            if ( c == alphabet[j] ) {
                index = (j-key) % (int)alphabet.size();
                
                index = checkIndex(index);
                
                plaintext.push_back(alphabet[index]);
                
                break;
            }
        }
    }
}

