#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include "game.h"
#include <fstream>
#include "get_valid_secret.hpp"
#include <cmath>
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
void game::set_puzzle(string puzzletext){

}
// ^ SEAN


/* print out comparison between this run and 
 * another run.
 * reference the class outline and display 
 * pertinent information via cout
 */
void game::display_comparison(game other){

}
// ^ SEAN


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
// i turned this into an include bc its really long 
// ^ ELI

// process input guess
int game::guess(string input_guess) {

  guesscount ++;
  guesslog.push_back(input_guess);

  if (input_guess == secret_word) {
    return GAME_WON;
  }
  if (guesscount == GUESSMAX) {
    return GAME_LOST;
  }
  return WRONG;

}

/* create a new game from some given text file.
 *
 * main goal is to find a string of 3 ^ GUESSMAX words
 * such that the last word is at the secret index in the file.
 */
game::game(string source_filename) {
  string chosen_secret_filename; // the file do you want to play the game on
  string puzzle_text; // the thing we will put to the console that includes a blacked out secret word at the end of the passage were reading

  secret activeSecret = get_valid_secret_index(chosen_secret_filename); // get a secret word form the source file
  int secret_index = activeSecret.index; // the position of the active secret in the secret index (indexlists)
  string secret_word = activeSecret.word; // the verbage of the secret word
  
  ifstream findSecret; // open a read stream 
  findSecret.open(source_filename + ".txt"); // read the sourcetext we will be finding the secret word
  int cursorPos = 0; // the word were on 
  while (findSecret.good() && cursorPos <= activeSecret.position) { // while the file is valid and we are not at the position of the secret word
  string tempString; // the string we are on in the sourcetext 
  findSecret >> tempString; // put the string in the sourcetext into tempstring 
  cursorPos ++; // itterate the word were on
  // the following defines the logic of the string shown to the user to educate their guess
  while ( (activeSecret.position - (3, GUESSMAX)) <= cursorPos < activeSecret.position ) // if the position of the string were on is within 3 ^ guessmax words before the secret word 
  puzzle_text.append(" " + tempString); // if so add it to the string people will see when they play
  }
  findSecret.close(); // close ya files
  puzzle_text.append(" "); // add a space at the end 
  for (char c: activeSecret.word) { // for however large the secret word is
  puzzle_text.append("_"); // black it out 
  }

  // secret_word = indexed word
  // set puzzle via puzzle text
  // remember to size puzzle text correctly (3 ^ GUESSMAX words)

  set_puzzle(puzzle_text);

}
// ^ ELI

// make new copy of some game with a blank history
game game::newgame_from_puzzle() {
  game blank_game;
  blank_game.puzzle = puzzle;
  return blank_game;
}
