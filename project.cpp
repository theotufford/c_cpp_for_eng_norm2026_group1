#include <cctype>
#include <cstdlib>
#include <iostream>
#include <map>
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
  static const int guess_max = 6;
  static const int game_length = 5;
  // ANSI color escape codes associated with their meaning
  string correct = "\033[42m";   // 42 - green background
  string contained = "\033[43m"; // 43 - yellow background
  string wrong = "\033[100m";    // 100 - bright black background

  const string white = "\033[47m";     // 47 - white background
  const string dark_text = "\033[30m"; // 30 - black text (in the foreground)
  const string reset_code = "\033[0m"; // 0 - reset (styles and colors)

  Board *opponent;
  int guess_counter = 0;
  int points = 0;
  vector<string> guess_history;
  string secret;
  // added this to use in coloring logic and to show what letters are remaining
  string letters_remaining = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  void printBoard();
  void printHelp();
  vector<string> get_guess_colors(string guess);
  void board_test();
  void toggle_colorblind();
  int verify_input();
};

void Board::toggle_colorblind() {
  correct = "\033[0;107m";   // - high intensity white
  contained = "\033[0;104m"; // - high intensity cyan
}

int Board::verify_input() {
  // check against wordlist
  // check word length
  return 0;
};

vector<string> Board::get_guess_colors(string guess) {
  vector<string> color_map;
  map<char, int> letter_counts; // map is form the data library, ai reccomended
                                // it for mapping values to characters

  for (int i = 0; i < secret.size(); i++) {
    letter_counts[secret.at(i)]++;
  }

  for (int position = 0; secret.size() > position; position++) {
    char guess_char = guess.at(position);

    if (secret.at(position) == guess_char) {
      color_map.push_back(correct);
      letter_counts[guess.at(position)]--;

    } else {
      color_map.push_back(wrong);

      for (int p_wrong = 0; secret.size() > p_wrong; p_wrong++) {

        if (guess_char == secret.at(p_wrong)) {
          letter_counts[guess.at(position)]--;

          if (letter_counts[guess.at(position)] >= 0) {
            color_map.at(position) = contained;
          }
        }
      }
    }
  }

  if (color_map.size() != game_length) {
    cout << "\n    error!!!"
         << "\n color map generated was the wrong length (" << color_map.size()
         << ")!\nit should have been " << game_length << "characters long"
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
    if (board_row < guess_history.size()) {

      string guess = guess_history.at(board_row);
      vector<string> guess_colors = get_guess_colors(guess);

      for (int letter_ind = 0; letter_ind < game_length; letter_ind++) {
        char letter = guess.at(letter_ind);
        string color_code = guess_colors.at(letter_ind);

        cout << color_code << dark_text << " " << (char)toupper(letter) << " "
             << reset_code << " ";
      }

    } else {
      // fill the rest of the board with gray empty tiles
      for (int letter_ind = 0; letter_ind < game_length; letter_ind++) {
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
  cout << bold << "╔═══════════════════════════════════════════════════╗"
       << reset_code << "\n";
  cout << bold << "║                     HELP MENU                     ║"
       << reset_code << "\n";
  cout << bold << "╠═══════════════════════════════════════════════════╣"
       << reset_code << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║                    How To Play                    ║"
       << reset_code << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║  Guess the Wordle in 6 tries.                     ║"
       << reset_code << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║   •  Each guess must be a valid 5-letter word.    ║"
       << reset_code << "\n";
  cout << bold << "║   •  The color of the tiles will change to show;  ║"
       << reset_code << "\n";
  cout << bold << "║       how close your guess was to the word.       ║"
       << reset_code << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║  After each guess, tiles show:                    ║"
       << reset_code << "\n";
  cout << "║  " << correct << dark_text << " A " << reset_code
       << " Correct spot                                 ║" << reset_code
       << "\n";
  cout << "║  " << contained << dark_text << " B " << reset_code
       << " Wrong spot                                   ║" << reset_code
       << "\n";
  cout << "║  " << wrong << dark_text << " C " << reset_code
       << " Not in the word                              ║" << reset_code
       << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║                                                   ║"
       << reset_code << "\n";
  cout << bold << "║  Enter '?' anytime for this menu.                 ║"
       << reset_code << "\n";
  cout << bold << "╚═══════════════════════════════════════════════════╝"
       << reset_code << "\n";
}

void stall() {
  string trash;
  cin.clear();
  cin.ignore();
  cout << "Enter to continue..." << endl;
  getline(cin, trash);
}

// test board rendering
void Board::board_test() {
  cout << "testing board rendering: " << endl;
  secret = "apple";
  guess_history = {"beeps", "farss"};
  printBoard();

  stall();
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
  if (choice == '?') {
    Board board;
    board.printHelp();
  }

  Board player1_board;
  Board player2_board;

  /*  dont worry about these & symbols for now,
   *  They are an operator that gets the memory address
   *  of the thing that comes after them
   *  the board object has an attribute named "opponent"
   *  to store a memory address associated with a different board
   *  to use the actual object at that memory address you have to
   *  "dereference" the pointer with a *
   *  a variable that stores a memory address is called a pointer
   *  becuase it "points" to a different region in memory
   */
  player1_board.opponent = &player2_board;
  player2_board.opponent = &player1_board;

  // when declaring a variable that holds a memory address you use
  // that same * symbol
  Board *active_board_ptr =
      &player2_board; // start on player 2 because the gameplay loop switches
                      // the player at the start

  int turn_counter = 0;
  while (true) {

    // fancy board swapping logic
    // the -> implicitly "dereferences" the thing before it and gets the
    // attribute after it from that object <memory address of object> ->
    // attribute
    // (*<memory address of object>).attribute
    active_board_ptr = active_board_ptr->opponent;

    /* when you declare a variable with the & in the declaration you are making
     * an alias variable. aliases essentially create a new name for whatever
     * they are assigned with
     */

    // "active_board is an alias to the dereferenced value of active_board_ptr"
    Board &active_board = *active_board_ptr;
    Board &opponent = *active_board.opponent;

    // everything past this point should make sense
    // ------- main gameplay loop code --------

    stall();
    system("clear");
    cout << "player " << turn_counter + 1 << "\'s turn" << endl;
    turn_counter = (turn_counter + 1) % 2; // flipflop between 1 and 0

    // assign new secret word
    // then continue and swap boards
    if (opponent.secret == "") {
      cout << "input secret word for your opponent: ";
      string tmp;
      cin >> tmp;
      cout << endl;
      // validate tmp
      opponent.secret = tmp;
      continue;
    }

    active_board.printBoard();
    string guess;
    while (true) {
      // loop unil valid guess given
      cout << "input guess: ";
      cin >> guess;
      cout << endl;
      if (guess.length() == Board::game_length) {
        break;
      }
      // TODO validate against word list
      cout << "not " << Board::game_length << " letters !" << endl;
    }

    // TODO validate guess for length and wordlist

    active_board.guess_history.push_back(guess);
    active_board.guess_counter++;
    system("clear");
    active_board.printBoard();
    bool out_of_guesses = opponent.guess_counter == Board::guess_max &&
                          active_board.guess_counter == Board::guess_max;
    // check if guess is secret
    if (guess == active_board.secret) {
      cout << "word guessed, got point!!" << endl;
      active_board.points++;
      stall();
      if (active_board.points == 2) {
        cout << "\n\n\t\tyou win!!!" << endl;
        break;
      }
      // check if out of guesses given secret hasnt been guessed
    } else if (out_of_guesses) {
      cout << "you guys suck" << endl;
      stall();
    } else {
      // if neither, continue with game loop
      continue;
    }

    // if either "out of guesses" or "secret guessed" this code
    // will execute because the "continue" is never executed
    player1_board.secret = "";
    player1_board.guess_counter = 0;
    player1_board.guess_history.clear();

    player2_board.secret = "";
    player2_board.guess_counter = 0;
    player2_board.guess_history.clear();
  }
}
