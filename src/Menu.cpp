//
// file Menu.cpp
//

#include "Menu.hpp"
#include "Caesar.hpp"
#include "RSA.hpp"
#include <iostream>
#include <sstream>

using namespace std;

void Menu::setEncoding() {
    // Get system's Active Code Page
    const int bufferSize = 128;
    FILE *pPipe;
    char psBuffer[bufferSize];
    string s;

    if( (pPipe = popen("chcp", "rt")) == NULL )
        exit(-1);

    while (fgets(psBuffer, bufferSize, pPipe)) {
        s += psBuffer;
    }

    pclose(pPipe);

    for (int i = 0; i < (int)s.size(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            originalCodePage += s[i];
        }
    }

    // Set desired encoding
    system(desiredCodePage.c_str());
}

void Menu::resetEncoding() {
    string s = "chcp " + originalCodePage;
    cout << "Setting previous code page." << endl;
    system(s.c_str());
}

Menu::Menu() {
    Caesar caesar;
    RSA rsa;
    vector<int> v;

	if (SET_ENCODING) {
		setEncoding();
	}

    do {
        printMenu();

        option = getOption(nOptions);

        switch (option) {
            case 1:
                cout << "Insert text to encrypt: ";
                caesar.setPlaintext(getString());

                cout << "Insert key: ";
                caesar.setKey(getInt());

                caesar.encrypt();

                cout << endl << "Encrypted text is: ";
                cout << RED_UL;
                cout << caesar.getCiphertext() << endl;
                cout << RESET;

                caesar.reset();

                toContinue();
                break;

            case 2:
                cout << "Insert text to decrypt: ";
                caesar.setCiphertext(getString());

                cout << "Insert key: ";
                caesar.setKey(getInt());

                caesar.decrypt();

                cout << endl << "Decrypted text is: ";
                cout << GREEN_UL;
                cout << caesar.getPlaintext() << endl;
                cout << RESET;

                caesar.reset();

                toContinue();
                break;

            case 3:
                if (debugIsOn()) {
                    rsa.printObject();
                }
                
                cout << "Insert text to encrypt: ";

                rsa.setPlaintext(getString());
                                
                rsa.setBlockM(rsa.stringToVectorInt(rsa.getPlaintext()));

                if (debugIsOn()) {
                    rsa.printVector(rsa.getBlockM());
                }

                rsa.encrypt();

                if (debugIsOn()) {
                    rsa.printVector(rsa.getBlockC());
                }

                rsa.setCipherText();

                cout << endl << "Encrypted text is: ";
                cout << RED_UL;
                cout << rsa.getCiphertext() << endl;
                cout << RESET;
                
                rsa.reset();

                toContinue();
                break;

            case 4:
                if (debugIsOn()) {
                    rsa.printObject();
                }

                cout << "Insert text to decrypt: ";

                rsa.setCiphertext(getString());

                rsa.ciphertextToBlockC();

                if (debugIsOn()) {
                    rsa.printVector(rsa.getBlockC());
                }

                rsa.decrypt();

                if (debugIsOn()) {
                    rsa.printVector(rsa.getBlockM());
                }

                rsa.setPlaintext(rsa.getBlockM());

                cout << endl << "Decrypted text is: ";
                cout << GREEN_UL;
                cout << rsa.getPlaintext() << endl;
                cout << RESET;

                rsa.reset();

                toContinue();
                break;
            
            case 5:
                rsa = RSA();

                if (debugIsOn()) {
                    rsa.printObject();
                }

                cout << "New pair of keys successfully generated." << endl;

                toContinue();
                break;

            case 6:
                switchDebug();
                break;

            default:
                break;
        }
    }
    while ( option != 0);
}

Menu::~Menu() {
	if (SET_ENCODING) {
		resetEncoding();
	}

    cout << "Farewell!" << endl;
}

int Menu::getOption(int n) {
    int newOption;
    stringstream ss;
    string input = "";

    ss.clear();
    ss.str("");

    cout << "\t\tOption: ";
    
    while (true) {
        if ( !getline(cin, input) ) {
            return -1;
        }

        ss.str(input);

        if (ss >> newOption && newOption >= 0 && newOption < n) {
            break;
        }

        cout << "Invalid option, try again" << endl;
        cout << "\t\tOption: ";

        ss.clear();
        ss.str("");
        input.clear();
    }

    return newOption;
}

void Menu::toContinue() {
    string input;
    cout << "Press ENTER to continue... ";
    getline(cin, input);
}

void Menu::printMenu() {
    string header = a3R + title + a3L;

    system(CLEAR);
    printCenteredTitle(header, menuLineLength);
    printMenuLimits(menuName);
    printAllLines();
    printMenuLimits("");
    cout << endl;
}

void Menu::printCenteredTitle(string title, int fullLength) {
    int i, empty_space = fullLength - charCount(title);

    if ( empty_space > 0 ) {
        for (i = 0; i < empty_space / 2; i++) {
            cout << " ";
        }
        cout << title;
        for (i = empty_space / 2; i < fullLength; i++) {
            cout << " ";
        }
    }
    else {
        cout << title;
    }
    cout << endl;
}

void Menu::printMenuLimits(string s) {
    int i, left_over = menuLineLength - charCount(s);

    for (i = 0; i < left_over / 2; i++) {
        cout << marginFill;
    }
    cout << s;
    for (i = 0; i < left_over / 2; i++) {
        cout << marginFill;
    }
    cout << endl;
}

void Menu::printOneLine(string s, int n) {
    int i;
    int leftOver = menuLineLength
                    - charCount(marginFill) * 2
                    - charCount(borderFill) * leftBorderLength
                    - charCount(s);

    cout << marginFill;

    for (i = 0; i < leftBorderLength; i++) {
        cout << borderFill;
    }

    cout << s;

    if ( leftOver + n >= 0 ) {
        for (i = 0; i < leftOver + n; i++) {
            cout << bodyFill;
        }
        cout << marginFill << endl;
    }
    else {
        cout << endl;
    }
}

void Menu::printAllLines() {
	string s, justFormat;

    printOneLine("");

	// 1 - Caesar's Cipher - Encrypt
	s = "1 - ";
	s.append(YELLOW).append("Caesar").append(RESET);
	s.append("'s Cipher - Encrypt");
	justFormat.clear();
	justFormat.append(YELLOW).append(RESET);
	printOneLine(s, justFormat.size());

	// 2 - Caesar's Cipher - Decrypt
	s = "2 - ";
	s.append(YELLOW).append("Caesar").append(RESET);
	s.append("'s Cipher - Decrypt");
	printOneLine(s, justFormat.size());

    printOneLine("");

	// 3 - RSA - Encrypt
	s = "3 - ";
	s.append(BLUE).append("RSA").append(RESET);
	s.append(" - Encrypt");
    printOneLine(s, justFormat.size());
    
	// 4 - RSA - Decrypt
	s = "4 - ";
	s.append(BLUE).append("RSA").append(RESET);
	s.append(" - Decrypt");
	printOneLine(s, justFormat.size());
    
	// 5 - RSA - Generate new pair of keys
	s = "5 - ";
	s.append(BLUE).append("RSA").append(RESET);
	s.append(" - Generate new pair of keys");
	printOneLine(s, justFormat.size());
    
	printOneLine("");
    
	// 6 - Switch debug mode
	printOneLine("6 - Switch debug mode");
    
	if(debugIsOn()) {
		// Debug: ON
		s = "Debug: ";
		s.append(GREEN).append("ON").append(RESET);
		justFormat.clear();
		justFormat.append(GREEN).append(RESET);
        printOneLine(s, justFormat.size());
    }
    else {
		// Debug: OFF
		s = "Debug: ";
		s.append(RED).append("OFF").append(RESET);
        justFormat.clear();
		justFormat.append(RED).append(RESET);
        printOneLine(s, justFormat.size());
    }

    printOneLine("");

	// 0 - Exit
    printOneLine("0 - Exit");
    
	printOneLine("");
}

// Count number of characters in a string
int Menu::charCount(string s) {

    int strLen = s.length();
    int u = 0;
    const char *c_str = s.c_str();
    int charCount = 0;

    while(u < strLen) {
        u += mblen(&c_str[u], strLen - u);
        charCount += 1;
    }
    
    return charCount;
}

int Menu::getInt() {
    int num = 0;
    stringstream ss;
    string input = "";

    ss.clear();
    ss.str("");

    while (true) {
        if ( !getline(cin, input) ) {
            return -1;
        }
        
        ss.str(input);

        if (ss >> num) {
            break;
        }

        cout << "Not a number, try again." << endl;

        ss.clear();
        ss.str("");
        input.clear();
    }

    return num;
}

string Menu::getString() {
    string input;
    getline(cin, input);

    while (input.size() == 0) {
        cout << "Not even a single character? Try again." << endl;
        getline(cin, input);
    }

    return input;
}