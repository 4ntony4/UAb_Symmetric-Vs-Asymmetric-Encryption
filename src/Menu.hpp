//
// file Menu.hpp
//

#ifndef MENU_HPP
#define MENU_HPP

#include <string>

#define CLEAR "@cls||clear"

#define SET_ENCODING 0

// Color reset
#define RESET       "\033[0m"

// Colors
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

// Underlined colors
#define RED_UL     "\033[4;31m"
#define GREEN_UL   "\033[4;32m"


using namespace std;

class Menu {
    private:
        int option;
        const int nOptions = 7; // 6 options + exit

        const string title = "CiPHeRS";
        const string menuName = " MENU ";
        const int menuLineLength = 44;
        const string marginFill = "/";
        const string bodyFill = " ";
        const string borderFill = " ";
        const int leftBorderLength = 3;
        const string a3R = ">>> ";  // 3 arrows to the right
        const string a3L = " <<<";  // 3 arrows to the left

        string originalCodePage;
        const string desiredCodePage = "chcp 850";

        bool debug = 0;

        void setEncoding();
        void resetEncoding();

    public:
        Menu();
        ~Menu();

        int getOption(int);
        static void toContinue();
        void printMenu();
        void printCenteredTitle(string, int);
        void printMenuLimits(string);
        void printOneLine(string, int = 0);
        void printAllLines();

        bool getDebug() { return debug; }
        bool debugIsOn() { return getDebug(); }
        
        void switchDebug() { debug ? debug = false : debug = true; }
        
        // Count number of characters in a string
        static int charCount(string);

        static int getInt();
        static string getString();
};

#endif /* MENU_HPP */