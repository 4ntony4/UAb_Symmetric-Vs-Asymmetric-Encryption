//
// file Cipher.hpp
//

#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <string>
#include <vector>

using namespace std;

class Cipher {
    protected:
        vector<char> alphabet;
        string plaintext;
        string ciphertext;

    public:
        Cipher();
        virtual ~Cipher() {};

        void setPlaintext(string s) { plaintext = s; }
        void setCiphertext(string s) { ciphertext = s; }

        string getPlaintext() { return plaintext; }
        string getCiphertext() { return ciphertext; }

        // delete values of plaintext and ciphertext
        virtual void reset();

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
        static unsigned int convertFromTwosComplement(int n, int bits);

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
        static int convertToTwosComplement(int n, int bits);

        virtual void encrypt() {};
        virtual void decrypt() {};

        void printAlphabet();
};

#endif /* CIPHER_HPP */