#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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
  const string white = "\033[47m";     // 47 - white background
  const string correct = "\033[42m";   // 42 - green background
  const string contained = "\033[43m"; // 43 - yellow background
  const string wrong = "\033[100m";    // 100 -bright black background
  const string dark_text = "\033[30m"; // 30 - black text (in the foreground)
  const string reset_code = "\033[0m"; // 0 - reset (styles and colors)

  vector<string> guess_history;
  string secret;
  void printBoard();
  vector<string> get_guess_colors(string guess);
  void board_test();
};

vector<string> Board::get_guess_colors(string guess) {
  vector<string> color_map;
  /*
  input: guess string

  return:
  vector of strings that encode the color of the tile at the
  same index in the guess string

  the colors are encoded as strings, hence the output vector being a vector of
  strings

  example:
  secret string:  apple
  guess: plume

  output:
  {contained, contained, wrong, wrong, correct}

  */
  return color_map;
}

// Print the board
void Board::printBoard() {
  cout << "\n";
  // iterate through every tile on board
  for (int guess_hist_ind = 0; guess_hist_ind < guess_max; guess_hist_ind++) {

    // if guesses remaining to be rendered
    if (guess_hist_ind < guess_history.size()) {

      // get string in guess history and its corresponding color coding
      string guess_string = guess_history.at(guess_hist_ind);
      vector<string> guess_color_map = get_guess_colors(guess_string);

      // iterate through guess string
      for (int guess_ind = 0; guess_ind < secret_length; guess_ind++) {

        // get individual letter and its color coding
        char letter = guess_string.at(guess_ind);
        string ansi_color_code = guess_color_map.at(guess_ind);

        // print letter to the screen
        cout << ansi_color_code << dark_text << " " << (char)toupper(letter)
             << " " << reset_code << " ";
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

// test board rendering
void Board::board_test() {
  cout << "testing board rendering: " << endl;
  secret = "plane";
  guess_history = {"crane", "slate"};
  printBoard();

  // blocking stall until enter from user
  int tmp;
  cin >> tmp;
  system("clear");
}

int main() {
  Board(player1_board);
  Board(player2_board);

  Board active_board = player1_board;

}
