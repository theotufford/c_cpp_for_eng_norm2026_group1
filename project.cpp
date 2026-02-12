#include <cctype>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Setting up board to have 6 rows and 5 columns for the tries(6) and word
// length(5)
const int guess_max = 6;
const int secret_length = 5;

// Tells the terminal to change the background to a differet color using ANSI
// Escape Codes
// \033 is command and tells it what comes next is a special character, [ starts
// the sequence numbers like 42 tell it to choose green background while the m
// ends the command sequence Always end with reset ANSI code to reset the color
// back to normal

const string white = "\033[47m";          // 47 - white background
const string correct = "\033[42m";          // 42 - green background
const string contained = "\033[43m";         // 43 - yellow background
const string wrong = "\033[100m";          // 100 -bright black background
const string dark_text_code = "\033[30m"; // 30 - black text (in the foreground)
const string reset_code = "\033[0m";      // 0 - reset (styles and colors)

// Print the board
void printBoard(vector<string> guess_history,
                vector<vector<string>> color_coding) {
  cout << "\n";
  // iterate through every tile on board
  for (int guess_hist_ind = 0; guess_hist_ind < guess_max; guess_hist_ind++) {

    // if guesses remaining to be rendered
    if (guess_hist_ind < guess_history.size()) {

      // get string in guess history and its corresponding color coding
      string guess_string = guess_history.at(guess_hist_ind);
      vector<string> guess_color_map = color_coding.at(guess_hist_ind);

      // iterate through guess string
      for (int guess_ind = 0; guess_ind < secret_length; guess_ind++) {

        // get individual letter and its color coding
        char letter = guess_string.at(guess_ind);
        string ansi_color_code = guess_color_map.at(guess_ind);


        // print letter to the screen
        cout << ansi_color_code << dark_text_code << " "
             << (char)toupper(letter) << " " << reset_code << " ";
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

// test to see if board works
int board_test() {
  vector<string> guess_history_test = {"crane", "slate"};
  vector<vector<string>> color_coding_test = {
      {contained, correct, correct, wrong, correct}, {contained, wrong, wrong, wrong, correct}};
  printBoard(guess_history_test, color_coding_test);
  return 0;
}

int main() {
  cout << "testing board rendering: " << endl;
  board_test();
  int tmp;
  cin >> tmp;
  system("clear");

  // variable setup
  vector<string> player1_guess_history;
  vector<string> player2_guess_history;

  string player1_secret = "";
  string player2_secret = "";

  int player1_score = 0;
  int player2_score = 0;

  // main loop
}
