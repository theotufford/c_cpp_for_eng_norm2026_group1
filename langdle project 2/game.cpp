#include "game.h"
#include <cctype>
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// was causing weird errors when I didnt directly declare this
game::game() : guesscount(0), guesslog({}), secret_word(""), winstate(START) {}
/* sets the puzzle vector from appropriately sized string
 *
 * puzzle vector should be a vector where each index has
 * 3 ^ index words in it that precede the secret word.
 * the number of total words in puzzle text should be
 * 3 ^ GUESSMAX
 *
 * input is a string of length 3 ^ GUESSMAX
 *
 * for example puzzle[0] is "word word secret word"
 *
 * this is iterated through in the gameplay loop to expose
 * progressively more information to the player
 */
void game::set_puzzle(string puzzletext) {
  string line;
  int word_count = 0;

  stringstream ss(puzzletext); // *Working text body*
  string word;
  vector<string> word_list;
  while (ss >> word) {
    word_count += 1;
    word_list.push_back(word); // word_list becomes {"the", "dog", "etc."}
  }
  for (int round_ind = 1; round_ind <= GUESSMAX; round_ind++) {
    string tempstring{};
    int guess_start = word_count - pow(3, round_ind);
    for (int word_ind = guess_start; word_ind < word_count; word_ind++) {
      tempstring += word_list.at(word_ind) + " ";
    }
    puzzle.push_back(tempstring);
  }
}
// ^ SEAN

/* print out comparison between this run and
 * another run.
 * reference the class outline and display
 * pertinent information via cout
 */
void game::display_comparison(game other) {
  int win_conclusion = game::GAME_WON;
  int loss_conclusion = game::GAME_LOST;
  int determine;
  vector<game> game_history;

  for (int i = 0; i < game_history.size(); i++) {
    // cout << "The real words are: " << puzzle.at(i) << endl;
    if (determine == 1) {
      // cout << "Game was won." << endl;
    } else {
      // cout << "Game was lost." << endl;
    }
  }
}

/* generates secret word index from local text file
 *
 * if ./indexlists/<filename> exists, pick a random index
 * from it and return it.
 * if not make it and then return a random index by the following method:
 * go through ./sources/<filename>.txt and find every instance of a word that:
 * - does not begin with a capital letter
 * - and contains puntuation immediately after it
 * log each index in ./indexlists/<filename> to minimize runtime.
 *
 */

bool validate_word(string word) {
  bool ends_in_punctuation = ispunct(word.back());
  bool longer_than_3 = word.size() > 3;
  bool notcaps = islower(word.at(0));

  return ends_in_punctuation && notcaps && longer_than_3;
}

int get_random_valid_index(string filename) {
  vector<int> valid_indecies{};
  fstream indexlist;
  fstream primary_source;
  primary_source.open("./sources/" + filename);
  // look for indexlist that indexes all the valid possible
  // secrets
  indexlist.open("./indexlists/" + filename);

  if (!indexlist.good()) {
    // if not found make it
    ofstream indexlist_file("./indexlists/" + filename);
    // index through primary source file, incrementing the index per word
    string currentword;
    int index = 0;
    while (primary_source >> currentword) {
      // dont pick a word less than the string length from the beginning
      if (index < pow(3, GUESSMAX)) {
        index++;
        continue;
      }
      // make sure word is a reasonable secret word
      bool valid = validate_word(currentword);
      if (valid) {
        // append index to valid indecies
        indexlist_file << " " + to_string(index);
        valid_indecies.push_back(index);
      }
      index++;
    }
    indexlist_file.close();
  }
  // if we didnt already populate the index vector by creating
  // the index file during this call we may do so now
  // by processing the index file
  if (valid_indecies.size() == 0) {
    int currentind;
    while (indexlist >> currentind) {
      valid_indecies.push_back(currentind);
    }
  }

  // randomly pick valid index from list
  cout << "secret indecies size: " << valid_indecies.size() << endl;
  indexlist.close();
  return valid_indecies.at(rand() % valid_indecies.size());
}
// ^ ELI + THEO

// process input guess
int game::guess(string input_guess) {

  guesscount++;
  guesslog.push_back(input_guess);

  if (input_guess == secret_word) {
    winstate = GAME_WON;
    return GAME_WON;
  }
  if (guesscount == GUESSMAX) {
    winstate = GAME_LOST;
    return GAME_LOST;
  }
  return WRONG;
}

/* create a new game from some given text file.
 *
 * main goal is to find a string of 3 ^ GUESSMAX words
 * such that the last word is at the secret index in the file.
 */
game::game(string source_filename)
    : guesscount(0), guesslog({}), secret_word(""), winstate(START) {
  string chosen_secret_filename; // the file do you want to play the game on
  string puzzle_text; // the thing we will put to the console that includes a
                      // blacked out secret word at the end of the passage
                      // were reading

  // the position of the active secret
  // in the secret index (indexlists)
  int secret_index = get_random_valid_index(source_filename);

  ifstream source_file; // open a read stream
  source_file.open("./sources/" + source_filename);
  // read the sourcetext we will be finding the secret word
  int cursorPos = 0; // the word were on
  string tempString; // the string we are on in the sourcetext
  while (source_file.good() &&
         cursorPos < secret_index) { // while the file is valid and we are not
                                      // at the position of the secret word
    // put the string in the sourcetext into tempstring
    source_file >> tempString;
    cursorPos++; // itterate the word were on
    // the following defines the logic of the string shown to the user to
    // educate their guess

    // if the position of the string
    // we're on is within 3 ^ guessmax
    // words before the secret word
    if (cursorPos >= (secret_index - pow(3, GUESSMAX))) {
      puzzle_text += tempString + " "; // if so add it to the string
                                       // people will see when they play
    }
  }
  // currently the cursor is on the secret word
  source_file >> secret_word;
  // remove the punctuation that is on its end by definition
  secret_word.pop_back();
  source_file.close(); // close ya files

  set_puzzle(puzzle_text);
}
// ^ ELI

// make new copy of some game with a blank history
game game::newgame_from_puzzle() {
  game blank_game;
  blank_game.puzzle = puzzle;
  return blank_game;
}
