//
// file Caesar.hpp
//

#ifndef CAESAR_HPP
#define CAESAR_HPP

#include "Cipher.hpp"

class Caesar : public Cipher {
    private:
        // check if index exists
        int checkIndex(int);
        
        int key;
        
    public:
        Caesar();
        ~Caesar() {};

        void setKey(int k) { key = k; }
        
        int getKey() { return key; }

        // delete values of variables plaintext, ciphertext and key
        void reset();

        void encrypt();
        void decrypt();
};

#endif /* CAESAR_HPP */