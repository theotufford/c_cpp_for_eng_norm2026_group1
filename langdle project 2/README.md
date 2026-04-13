# langdle - Project 2
## initial brainstorm
Langdle is a game where you fight back and take AI's job, seeing how well you are able to do token prediction on literature
samples from project gutenberg.

The idea of the game is that, given some language data, you are trying to guess the next token. Much like how a language model does.
Every time you fail you are given more words you have 5 (arbitrary) attempts.

### extensions
- local AI model to compete with
    - ran out of time on this :(
    - https://github.com/ggml-org/llama.cpp
## systems
### file handling
- take text file input
- search for words that are at the end of sentence
- pick random sentence end word 
- load previous 100 words as problem section
### input handling
- check if correct word
- display new tokens or give victory

Documentation for filesystem used in the main():
https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
https://en.cppreference.com/w/cpp/filesystem.html
https://www.geeksforgeeks.org/cpp/file-system-library-in-cpp-17/


# final notes
## basic outline
The game is played by essentially picking a random word within a text file, and then giving you 3 ^ (guess count) words before
that word to help you guess that word. 
## failures
There was originally a plan to implement an LLM to also play the game so you could see how you compare to its guesses given the 
same information. However, this was a me (theo) thing and I just ran out of time, you may note in the code that a function
originally attributed as a task to sean was incomplete but this isnt really his fault because there was no real use for it 
outside of comparing a played AI run and your run.

also this was probably a bad place to use a class. Should've taken more time and gone the functional programming approach.
That is to say that Dijkstra would probably kill me with a hammer if he had to read through this code.

> "Object-oriented programming is an exceptionally bad
>  idea which could only have originated in California"
>  -Edsger Dijkstra

another thing is that my outlines were somewhat confusing. also some of the generalized structure was changed
from the original outline.

## The original outline that was given to the team:
### main.cpp
``` cpp 
// use a local AI model to play the game
game llmsolve(game player_game) {
  game llmgame = player_game.newgame_from_puzzle();

  // make the AI play

  return llmgame;
}
// ^ THEO

// main gameplay loop
game humansolve(string filename) {

  // explicitly calling the 
  // constructor here makes it a 
  // bit clearer
  game humangame = game(filename);

  // make the human play 
  // take input guesses using the guess function
  // to process them
  // note the utility of the enum defined within 
  // the game class

  return humangame;
}
// ^ THEO

// the main role of this is to repeat the game until the user quits
// should also iterate through all the local text files and randomly
// pick one to build that round of the game from
// then compare the two games and repeat
int main() {
  while (true) {
    string filename;

    // display general game title and information

    // pick file logic goes here
    // iterate through files in "sources"
    // and randomly pick one

    game human_game = humansolve(filename);

    game llm_game = llmsolve(human_game);

    human_game.display_comparison(llm_game);

  }
  return 0;
}
// ^ FARUK
```
### game.h
``` cpp
using namespace std;
#define GUESSMAX 5
// provides abstraction for playing the game so that
// progress is automatically tracked
class game {
  private:
  vector<string> puzzle = {"empty"};
  int guesscount = 0;
  vector<string> guesslog = {};
  string secret_word;

  public:
  int guess(string input_guess);
  enum {
    WRONG,
    GAME_WON,
    GAME_LOST
  };
  void display_comparison(game other);
  void set_puzzle(string puzzletext);
  game(); // allow for making empty new games
  game(string source_filename);
  game newgame_from_puzzle();
  game get_testgame();

};
```
### game.cpp
```cpp 
/* sets the puzzle vector from appropriately sized string
 *
 * puzzle vector should be a vector where each index has 
 * 3 ^ index words in it that precede the secret word.
 * the number of total words in puzzle text should be 
 * 3 ^ GUESSMAX
 *
 * input is a string of length 3 ^ GUESSMAX
 *
 * for example puzzle[0] is "word word word secret word"
 *
 * this is iterated through in the gameplay loop to expose 
 * progressively more information to the player
 *
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
```
