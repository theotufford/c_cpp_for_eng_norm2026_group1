#include "game.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
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
 * for example puzzle[0] is "word word secret-word"
 *
 * the puzzle vector is iterated through in the gameplay loop to expose
 * progressively more information to the player
 */
void game::set_puzzle(string puzzletext) {
  const int puzzle_wordcount = pow(3, GUESSMAX);
  // iterate through each possible round (start at 1 so initial reveal is 3)
  for (int round_ind = 1; round_ind <= GUESSMAX; round_ind++) {
    string given_string =
        ""; // string that contains the given info for this round
    string tempstring;
    stringstream puzzle_stream(
        puzzletext); // create new stream from given text
                     // this has to be done every time because
                     // the extraction operator consumes data

    // find where the given clue string begins in the word list
    int given_start_index = puzzle_wordcount - pow(3, round_ind);
    // consume the buffer into oblivion until you reach the right index
    for (int word_ind = 0; word_ind < given_start_index; word_ind++) {
      puzzle_stream >> tempstring;
    }

    while (puzzle_stream >> tempstring) {
      given_string += tempstring + " ";
    }
    // add given string to its puzzle index in the puzzle vector
    puzzle.push_back(given_string);
  }
}
// ^ SEAN
/* this function is maybe sort of a lost cause and an endeavor in
 * over-delegation. That one is on me. I also gave slightly poorly worded
 * directions which meant that I ended up re-writing this one because I didn't
 * clarify the context of this function well enough in the outline.
 * Also, the original solution I finished and the one I turned in was functional
 * but had a really logic flow that unnecessarily reversed and un-reversed the
 * given string.
 * - Theo
 */

// validate whether word is a valid secret word choice
bool validate_word(string word) {

  // commas are sort of against the spirit of the game 
  // even though this makes it easier.
  bool punctuated = ispunct(word.back()) && word.back() != ',';
  bool capitalized = isupper(word.front());

  // return early to avoid unnecessarily iterating over a word if word isnt promising
  if ( !punctuated || capitalized) {
    return false;
  }

  // check if promising word contains other punctuation and invalidate it if so
  word.pop_back();
  for (char letter: word) {
    if (!isalpha(letter)) {
      return false;
    }
  }

  // word is otherwise valid!
  return true;
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

int get_random_valid_index(string filename) {
  // vector to be populated with the word indices choose a random index from
  vector<int> valid_indices{};
  // fstream for the cached index list
  fstream indexlist;
  // fstream for the primary text source
  fstream primary_source;
  primary_source.open("./sources/" + filename);
  indexlist.open("./indexlists/" + filename);

  // if we didnt find a cached index list for the named file
  if (!indexlist.good()) {
    // make said file
    ofstream indexlist_file("./indexlists/" + filename);
    string currentword;
    int word_ind = 0;
    // index through primary source file, incrementing the index per word
    while (primary_source >> currentword) {
      // dont pick a word less than the string length from the beginning
      if (word_ind < pow(3, GUESSMAX)) {
        word_ind++;
        continue;
      }
      // make sure word is a reasonable secret word
      bool valid = validate_word(currentword);
      if (valid) {
        // append word index to valid indices
        indexlist_file << " " + to_string(word_ind);
        valid_indices.push_back(word_ind);
      }
      word_ind++;
    }
    // close ya files
    indexlist_file.close();
    primary_source.close();
  }
  // if we didnt already populate the index vector by creating
  // the index file during this call we may do so now
  // by processing the index file
  if (valid_indices.size() == 0) {
    int currentind;
    while (indexlist >> currentind) {
      valid_indices.push_back(currentind);
    }
  }

  // randomly pick valid index from list
  cout << "secret indices size: " << valid_indices.size() << endl;
  indexlist.close();
  return valid_indices.at(rand() % valid_indices.size());
}
// ^ ELI
/* Eli slightly overcomplicated this on himself but I used pretty much the same
 * logic and just trimmed out a couple of things he did that created too much
 * technical debt, I would've left it but there was a weird parsing error that I
 * literally couldn't figure out how to fix because of the tech debt. At the end
 * of the day its on me as a leader to have given him a more clear outline
 * - Theo
 */

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
  punct = secret_word.back();
  secret_word.pop_back();
  source_file.close(); // close ya files

  set_puzzle(puzzle_text);
}
// ^ ELI
/*
 * basically no cleanup required on this one from me, I just replaced and
 * simplified some logic because Eli's original solution for the
 * "get_random_valid_index" function returned a struct and used some logic from
 * that, which was no longer needed once the get random function was modified.
 *  - Theo
 */
