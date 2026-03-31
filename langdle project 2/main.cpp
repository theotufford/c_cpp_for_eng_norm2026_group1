#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include "game.cpp"

using namespace std;

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
//
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
