//
// file RSA.hpp
//

#ifndef RSA_HPP
#define RSA_HPP

#include "Cipher.hpp"

using namespace std;

class RSA : public Cipher {
    private:
        // two prime numbers
        int p, q;

        // public, calculated
        int n;

        // public, chosen
        int e;

        // private, calculated
        int d;

        int phi_n;
        int publicKey[2];
        int privateKey[2];

        // plaintext blocks
        vector<int> blockM;

        // cyphertext blocks
        vector<int> blockC;

        // Number of digits of n
        int maxNumDigits;

        // Number of chars in an element of blockC
        int nCharsInC;

        // Select the variable e
        int random_e();

        // Calculate the variable d
        int compute_d();
        
    public:
        RSA();
        ~RSA() {};

        void setBlockM(vector<int> m) { blockM = m; }    
        void setBlockC(vector<int> c) { blockC = c; }
        void setPlaintext(string s) { plaintext = s; }
        
        // Set plaintext from blockM
        void setPlaintext(vector<int>);

        // Set ciphertext from blockC
        void setCipherText();

        vector<int> getBlockM() { return blockM; }
        vector<int> getBlockC() { return blockC; }

        int getMaxNumDigits() { return maxNumDigits; }

        int getNCharsInC() { return nCharsInC; }
        
        // Delete values of variables plaintext, ciphertext, blockM and blockC
        void reset();

        void encrypt();
        void decrypt();

        // Return the number of digits in a given integer
        static int getNumDigits(int);
        
        // Generate pseudo-random prime number
        static int generatePrime();

        // Primality test using 6k+-1 optimization
        static bool isPrime(int);

        // Decimal to Binary Converting
        static string toBinary(int);

        // Convert a string into a vector<int>
        // All elements of the vector will be converted from a two's complement value to the respective unsigned value
        static vector<int> stringToVectorInt(string);

        // To convert an element of blockC into a vector<int> with size nCharsInC
        vector<int> intToVector(int);

        // Convert a vector<int> into a string
        string vectorToString(vector<int>);

        // Convert ciphertext into vector<int> blockC
        // Ciphertext will be splitted acording to variable nCharsInC
        void ciphertextToBlockC();

        // The values to be decrypted are not the ones corresponding to each character, but the indexes of the alphabet that correspond to those values
        vector<int> transformVectorInt(vector<int>);

        // Find the index of an element of alphabet
        int findAlphabetIndex(int);

        // Convert a string of a binary number to a vector<int>
        // The binary number will be in reverse order
        static vector<int> binaryToVector(string);

        // phi function of 2 primes
        static int phi_2primes(int, int);

        // Algorithm for computing a^b mod n
        static int ab_mod_n(int a, int b, int n);

        // Algorithm to find the GCD of 2 numbers
        // (Greatest Common Divisor)
        static int gcd(int, int);

        // Computes the multiplicative inverse of u mod v, u^-1 (mod v).
        // Returns either the inverse as a positive integer less than v, or zero if no inverse exists.
        static unsigned int modularInverse(unsigned int u, unsigned int v);

        void printObject();

        void printVector(vector<int> v);
};

#endif /* RSA_HPP */