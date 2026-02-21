#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;



/* ANSI escape codes
   Tells the terminal to change the background to a differet color using ANSI
   Escape Codes
   \033 is command and tells it what comes next is a special character, [
   starts the sequence numbers like 42 tell it to choose green background
   while the m ends the command sequence Always end with reset ANSI code to
   reset the color back to normal
*/

class Board {
public:
  const int guess_max = 6;
  const int secret_length = 5;
  // ANSI color escape codes associated with their meaning
  string correct = "\033[42m";   // 42 - green background
  string contained = "\033[43m"; // 43 - yellow background
  string wrong = "\033[100m";    // 100 - bright black background

  string white = "\033[47m";     // 47 - white background
  string dark_text = "\033[30m"; // 30 - black text (in the foreground)
  string reset_code = "\033[0m"; // 0 - reset (styles and colors)

  vector<string> history;
  string secret;
  void printBoard();
  void printHelp();
  vector<string> get_guess_colors(string guess);
  void board_test();
  void toggle_colorblind();
  void toggle_windowsDisplay();
};

// Windows terminal(powershell) does not support ANSI codes so plain text instead
void Board::toggle_windowsDisplay(){

  if (windowsDisplay) {
    // Plain text for windowsOS instead of ANSI
    correct = "correct";
    contained = "contained";
    wrong = "wrong";
    dark_text = "";
    reset_code = "";
  } else {
    // ANSI mode for macOS/linux
      correct = "\033[42m";   // 42 - green background
      contained = "\033[43m"; // 43 - yellow background
      wrong = "\033[100m";    // 100 - bright black background
      dark_text = "\033[30m"; // 30 - black text (in the foreground)
      reset_code = "\033[0m"; // 0 - reset (styles and colors)
  }
}

void Board::toggle_colorblind(){
  correct = "\033[0;107m";   // - high intensity white
  contained = "\033[0;104m"; // - high intensity cyan
}

vector<string> Board::get_guess_colors(string guess) {
  vector<string> color_map;
    

map<char, int> letter_counts; // map is form the data library, ai reccomended it for mapping values to characters

  
  for (int i = 0; i < secret.size(); i++) { //create a key of values with the number of times each letter appears in secret
  letter_counts[secret.at(i)] ++;
  }

  for(int position = 0; secret.size() > position; position++) { 
    char guess_char = guess.at(position);

    if(secret.at(position) == guess_char) {                     
      color_map.push_back(correct);
      letter_counts[guess.at(position)] --;

    } else {
      color_map.push_back(wrong); 

      for (int p_wrong = 0; secret.size() > p_wrong; p_wrong++) {

        if (guess_char == secret.at(p_wrong)) {
        letter_counts[guess.at(position)] --;

          if (letter_counts[guess.at(position)] >= 0) {
            color_map.at(position) = contained;
            }
          }
        }
      }
    }
  

  
  if (color_map.size() != secret_length) {
    cout << "\n    error!!!"
         << "\n color map generated was the wrong length (" << color_map.size()
         << ")!\nit should have been " << secret_length << "characters long"
         << endl;
  }
  return color_map;
}

// Print the board
void Board::printBoard() {
  cout << "\n";
  // iterate through every tile on board
  // if guesses remaining to be rendered
  // get string in guess history and its corresponding color coding
  // iterate through guess string
  // get individual letter and its color coding
  // print tile to the screen

  for (int board_row = 0; board_row < guess_max; board_row++) {

    if (board_row < history.size()) {

      string guess = history.at(board_row);
      vector<string> guess_colors = get_guess_colors(guess);

      for (int letter_ind = 0; letter_ind < secret_length; letter_ind++) {
        char letter = guess.at(letter_ind);
        string color_code = guess_colors.at(letter_ind);

        if(windowsDisplay){
          cout << (char)toupper(letter) << " [" << color_code << "] ";
        } else {
        cout << color_code << dark_text << " " << (char)toupper(letter)
             << " " << reset_code << " ";
            }
      }

    } else {
      // fill the rest of the board with gray empty tiles
      for (int letter_ind = 0; letter_ind < secret_length; letter_ind++) {
        cout << wrong << "   " << reset_code << " ";
      }
    }
    cout << "\n\n";
  }
}
// print the help menu using ANSI escape codes
void Board::printHelp() {
  string bold = "\033[1m";
  string reset_code = "\033[0m";

  cout << "\n";
    cout << bold << "╔═══════════════════════════════════════════════════╗" << reset_code << "\n";
    cout << bold << "║                     HELP MENU                     ║" << reset_code << "\n";
    cout << bold << "╠═══════════════════════════════════════════════════╣" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║                    How To Play                    ║" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║  Guess the Wordle in 6 tries.                     ║" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║   •  Each guess must be a valid 5-letter word.    ║" << reset_code << "\n";
    cout << bold << "║   •  The color of the tiles will change to show;  ║" << reset_code << "\n";
    cout << bold << "║       how close your guess was to the word.       ║" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║  After each guess, tiles show:                    ║" << reset_code << "\n";
    cout << "║  "<< correct << dark_text << " A " << reset_code << " Correct spot                                 ║" << reset_code << "\n";
    cout << "║  "<< contained << dark_text << " B " << reset_code << " Wrong spot                                   ║" << reset_code << "\n";
    cout << "║  "<< wrong << dark_text << " C " << reset_code << " Not in the word                              ║" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║                                                   ║" << reset_code << "\n";
    cout << bold << "║  Enter '?' anytime for this menu.                 ║" << reset_code << "\n";
    cout << bold << "╚═══════════════════════════════════════════════════╝" << reset_code << "\n";


}

// test board rendering
void Board::board_test() {
  cout << "testing board rendering: " << endl;
  secret = "apple";
  history = {"beeps", "farss"};
  printBoard();

  // blocking input stall
  int tmp;
  cin >> tmp;
}

int main() {

  // title of game
  string bold = "\033[1m";
  string reset = "\033[0m";
   
  cout << "\n";
  cout << bold << "╔═══════════════════════════════╗" << reset << "\n";
  cout << bold << "║            WORDLE             ║" << reset << "\n";
  cout << bold << "╚═══════════════════════════════╝" << reset << "\n\n";

  // asks input from player to display help menu
  char choice;
  cout << "Enter a word or ? for help: ";
  cin >> choice;
  if(choice == '?') {
    Board board;
    board.printHelp();
  }

  Board (player2_board);

  player2_board.toggle_colorblind();
  player2_board.board_test();

  Board (player3_board);
  player3_board.toggle_colorblind();
  player3_board.board_test();

}

