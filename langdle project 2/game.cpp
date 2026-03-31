#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include "game.h"

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
int get_valid_secret_index(string filename) {
  int chosen_index;
  return chosen_index;
}
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
  string chosen_secret_filename;
  string puzzle_text;

  int secret_index = get_valid_secret_index(chosen_secret_filename);

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
