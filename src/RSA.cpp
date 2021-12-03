//
// file RSA.cpp
//

#include "RSA.hpp"
#include <random>
#include <chrono>

// Select the variable e
int RSA::random_e() {
    bool checkGCD = false;
    int num;

    //  1 < e < phi_n
    int min = 2;
    int max = phi_n - 1;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    
    default_random_engine generator(seed);
    
    uniform_int_distribution<int> distribution(min, max);

    while (checkGCD == false) {
        num = distribution(generator);

        if (gcd(phi_n, num) == 1) {
            checkGCD = true;
        }
    }

    return num;
}

// Compute the variable d
int RSA::compute_d() {
    return (int)modularInverse((unsigned int)e, (unsigned int)phi_n);
}

RSA::RSA() {
    // change Cipher alphabet so it contains 100 elements
    for (int i = 128; i < 133; i++) {
        alphabet.push_back(i);
    }

    p = q = generatePrime();

    while (p == q) {
        q = generatePrime();
    }

    n = p * q;

    // ensure that M < n
    if (256 >= n) {
        p = 89, q = 23, n = p * q;
    }

    phi_n = phi_2primes(p ,q);

    maxNumDigits = getNumDigits(n-1);

    nCharsInC = (int)ceil((float)maxNumDigits/(float)2);
    
    e = random_e();
    d = compute_d();

    // if no inverse exists 
    if (d == 0) {
        e = 433, d = 465;
    }

    publicKey[0] = e;
    publicKey[1] = n;

    privateKey[0] = d;
    privateKey[1] = n;
}

// Set plaintext from blockM
void RSA::setPlaintext(vector<int> v) {
    char c;
    plaintext.clear();
    
    for (int i = 0; i < (int)v.size(); i++) {
        c = v[i];
        plaintext += c;
    }
}

// Set ciphertext from blockC
void RSA::setCipherText() {
    string str;
    vector<int> v;

    for (int i = 0; i < (int)blockC.size(); i++) {
        v = intToVector(blockC[i]);
        str += vectorToString(v);
    }

    ciphertext = str;
}

// Delete values of variables plaintext, ciphertext, blockM and blockC
void RSA::reset() {
    Cipher::reset();
    blockM.clear();
    blockC.clear();
}

void RSA::encrypt() {
    blockC.clear();

    for (int i = 0; i < (int)blockM.size(); i++) {
        blockC.push_back( ab_mod_n(blockM[i], e, n) );
    }
}

void RSA::decrypt() {
    blockM.clear();

    for (int i = 0; i < (int)blockC.size(); i++) {
        blockM.push_back( ab_mod_n(blockC[i], d, n) );
    }
}

// Return the number of digits in a given integer
int RSA::getNumDigits(int n) {
    return (int)(log10(n) + 1);
}

// Generate pseudo-random prime number
int RSA::generatePrime() {
    int n;
    bool prime = false;

    int min = 23;
    int max = 97;

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    default_random_engine generator(seed);
    
    uniform_int_distribution<int> distribution(min, max);

    while (!prime) {
        // random number between min and max
        n = distribution(generator);

        // if it's an odd number
        if ( n % 2 != 0 ) {
            if (isPrime(n)) {
                prime = true;
            }
        }
    }

    return n;
}

// Primality test using 6k+-1 optimization
bool RSA::isPrime(int n) {
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}

// Decimal to Binary Converting
string RSA::toBinary(int n) {
    string s;

    while (n != 0) {
        s = ( n%2 == 0 ? "0" : "1" ) + s; n /= 2;
    }
    return s;
}

// Convert a string into a vector<int>
// All elements of the vector will be converted from a two's complement value form to the respective unsigned value form
vector<int> RSA::stringToVectorInt(string s) {
    vector<int> v;

    for ( int i = 0; i < (int)s.size(); i++) {
        v.push_back(convertFromTwosComplement(s[i], 8));
    }

    return v;
}

// To convert an element of blockC into a vector<int> with size nCharsInC
vector<int> RSA::intToVector(int n) {
    int nDigits = getNumDigits(n);
    string word = to_string(n);
    string subWord;
    vector<string> vs;
    vector<int> vi;

    //position of last digit
    int pos = nDigits - 1;
    
    while (pos - 2 >= 0) {
        subWord = word.substr(pos - 1, string::npos);
        vs.insert(vs.begin(), subWord);
        word.erase(pos - 1, string::npos);
        pos -= 2;
    }
    vs.insert(vs.begin(), word);

    while ((int)vs.size() != nCharsInC) {
        vs.insert(vs.begin(), "0");
    }

    for (int i = 0; i < (int)vs.size(); i++) {
        vi.push_back(stoi(vs[i]));
    }

    return vi;
}

// Convert a vector<int> into a string
string RSA::vectorToString(vector<int> v) {
    char c;
    string s;
    for (int i = 0; i < (int)v.size(); i++) {
        c = alphabet[v[i]];
        s += c;
    }

    return s;
}

// Convert ciphertext into vector<int> blockC
// Ciphertext will be splitted acording to variable nCharsInC
void RSA::ciphertextToBlockC() {
    vector<string> vs;
    vector<int> vi;
    string subStr, intStr;
    int n;

    for (int i = 0; i < (int)ciphertext.size(); i += nCharsInC) {
        subStr = ciphertext.substr(i, nCharsInC);
        vs.push_back(subStr);
    }

    for (int i = 0; i < (int)vs.size(); i++) {
        vi = stringToVectorInt(vs[i]);

        //transform vector vi to correct values
        vi = transformVectorInt(vi);

        for (int j = 0; j < (int)vi.size(); j++) {
            if (vi[j] > 9) {
                intStr += to_string(vi[j]);
            }
            else {
                intStr += "0" + to_string(vi[j]);
            }
        }
        n = stoi(intStr);
        blockC.push_back(n);
        intStr.clear();
    }
}

// The values to be decrypted are not the ones corresponding to each character, but the indexes of the alphabet that correspond to those values
vector<int> RSA::transformVectorInt(vector<int> v){
    int num;

    for(int i = 0; i < (int)v.size(); i++) {
        num = findAlphabetIndex(convertToTwosComplement(v[i], 8));
        v[i] = num;
    }

    return v;
}

// Find the index of an element of alphabet
int RSA::findAlphabetIndex(int n) {
    for (int i = 0; i < (int)alphabet.size(); i++) {
        if (n == alphabet[i])
            return i;
    }
    return -1;
}

// Convert a string of a binary number to a vector<int>
// The binary number will be in reverse order
vector<int> RSA::binaryToVector(string s) {
    vector<int> v;

    for (int i = (int)s.size() - 1; i >= 0; i--) {
        v.push_back( (int)(s[i] - '0') );
    }

    return v;
}

// phi function of 2 primes
int RSA::phi_2primes(int a, int b) {
    return (a-1)*(b-1);
}

// Algorithm for computing a^b mod n
int RSA::ab_mod_n(int a, int b, int n) {
    int c = 0;
    unsigned long long int f = 1;
    vector<int> v = binaryToVector(toBinary(b));
    int k = (int)v.size() - 1;
    
    for (int i = k; i >= 0; i--) {        
        c *= 2;
        f = (f * f) % n;

        if(v[i] == 1) {
            c += 1;
            f = (f * a) % n;
        }
    }

    return f;
}

// Algorithm to find the GCD of 2 numbers
// (Greatest Common Divisor)
int RSA::gcd(int a, int b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a%b);
}

// Computes the multiplicative inverse of u mod v, u^-1 (mod v).
// Returns either the inverse as a positive integer less than v, or zero if no inverse exists.
unsigned int RSA::modularInverse(unsigned int u, unsigned int v) {
    unsigned int inv, u1, u3, v1, v3, t1, t3, q;
    int iter;
    // Step X1. Initialise
    u1 = 1;
    u3 = u;
    v1 = 0;
    v3 = v;
    // Remember odd/even iterations
    iter = 1;
    // Step X2. Loop while v3 != 0
    while (v3 != 0) {
        // Step X3. Divide and "Subtract"
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
        // Swap
        u1 = v1; v1 = t1; u3 = v3; v3 = t3;
        iter = -iter;
    }
    // Make sure u3 = gcd(u,v) == 1
    if (u3 != 1)
        return 0;   // Error: No inverse exists
    // Ensure a positive result
    if (iter < 0)
        inv = v - u1;
    else
        inv = u1;
    return inv;
}

void RSA::printObject() {
    printf("\n");
    printf("%5s = %d\n", "p", p);
    printf("%5s = %d\n", "q", q);
    printf("%5s = %d\n", "n", n);
    printf("\n");
    printf("%5s = %d\n", "e", e);
    printf("%5s = %d\n", "d", d);
    printf("%5s = %d\n", "phi_n", phi_n);
    printf("\n");
}

void RSA::printVector(vector<int> v) {
    int nColumns = 4;

    printf("\n");

    for (int i = 0; i < (int)v.size(); i++) {
        if ( (i % nColumns) == nColumns - 1 ) {
            printf("[%8d]\n", v[i]);
        }
        else {
            printf("[%8d] ", v[i]);
        }
    }

    printf("\n");
}