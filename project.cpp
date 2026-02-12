#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Setting up board to have 6 rows and 5 columns for the tries(6) and word length(5)
const int rows = 6;
const int cols= 5;


// Tells the terminal to change the background to a differet color using ANSI Escape Codes
// \033 is command and tells it what comes next is a special character, [ starts the sequence
// numbers like 42 tell it to choose green background while the m ends the command sequence
// Always end with reset ANSI code to reset the color back to normal
string green = "\033[42m"; // 42 is the ANSI color code for a green background
string yellow = "\033[43m"; // 43 is the ANSI color code for a yellow background
string gray = "\033[100m"; // 100 is the ANSI color code for a bright black background
string black = "\033[30m"; // 30 is the ANSI color code for a black text (in the foreground)
string white = "\033[47m"; // 47 is the ANSI color code for a white background
string reset = "\033[0m"; // 0 is the ANSI color code for reset all modes(styles and colors) 



// Print one colored tile
void printTile(char letter, char status) {
    string color;
        if (status == 'G')
            color = green;
        else if (status == 'Y')
            color = yellow;
        else
            color = gray;
    
        cout << color << black
            << " " << (char)toupper(letter) << " "
            << reset;       
}


// Print the board
void printBoard(vector<string> guesses, vector<string> result) {
    
    cout << "\n";
    
    for (int r = 0; r < rows; r++) {

        if (r < (int)guesses.size()) {
            for (int c = 0; c < cols; c++) {
                printTile(guesses[r][c], result[r][c]);
                cout << " ";
            }
        } else {
            for (int c = 0; c < cols; c++){
                cout << gray << "   " << reset << " ";
            }
        } 
       cout << "\n\n"; 
    }
}

//test to see if board works
int main() {
    vector<string> guesses = {"crane", "slate"};
    vector<string> result  = {"GBYBG", "BBYBG"};
    printBoard(guesses, result);
}