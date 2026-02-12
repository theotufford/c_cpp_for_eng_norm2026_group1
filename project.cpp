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
  string correct = "\033[42m";   // 42 - green background
  string contained = "\033[43m"; // 43 - yellow background
  string wrong = "\033[100m";    // 100 - bright black background

  const string white = "\033[47m";     // 47 - white background
  const string dark_text = "\033[30m"; // 30 - black text (in the foreground)
  const string reset_code = "\033[0m"; // 0 - reset (styles and colors)

  vector<string> history;
  string secret;
  void printBoard();
  vector<string> get_guess_colors(string guess);
  void board_test();
  void toggle_colorblind();
};

void Board::toggle_colorblind(){
  correct = "\033[0;107m";   // - high inensity white
  contained = "\033[0;104m"; // - high inensity cyan
}

vector<string> Board::get_guess_colors(string guess) {
  vector<string> color_map;
  /*
  input: guess string

  return:
  vector of strings that encode the color of the tile at the
  same index in the guess string

  the colors are encoded as strings, hence the output vector being a vector of
  strings

  */

  // FOR EXAMPLE/TESTING:
  // (declared in the board test function)
  // secret = apple
  // guess_history = {"plume", "beeps"};
  color_map = {contained, contained, wrong, wrong, correct}; // correct for plume

  // on successful design it should output the color map for the second guess as
  // {wrong, contained, contained, contained, wrong }

  // freindly error check
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

        cout << color_code << dark_text << " " << (char)toupper(letter)
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
  secret = "apple";
  history = {"plume", "beeps"};
  printBoard();

  // blocking input stall
  int tmp;
  cin >> tmp;
}

int main() {
  Board(player2_board);

  player2_board.toggle_colorblind();
  player2_board.board_test();

  Board(player3_board);
  player3_board.toggle_colorblind();
  player3_board.board_test();
}
