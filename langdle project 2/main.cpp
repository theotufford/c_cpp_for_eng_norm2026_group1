#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include "game.cpp"
#include <iostream>
#include <filesystem>
#include <ctime>

using namespace std;
using namespace std::filesystem;

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
  srand(time(nullptr)); // seeds rand() so same file isnt opened everytime
  
  char playAgain;       // tracks whether user want to play again
  do {
    // display general game title and information
    cout << "====================================" << endl;
    cout << "                      LANGDALE" << endl;
    cout << "====================================" << endl;
    cout << "   Can you fight back and take AI's job?" << endl;
    cout << "   Or will you fail and be the ultimate chud?" << endl;
    cout << "   Rules:" << endl;
    cout << "   Guess the next word in a text pulled from a book." << endl;
    cout << "   You have 5 attempts. Good Luck!" << endl;
    cout << endl;

    // scans the ./sources/ directory for .txt files.
    path directorypath = "./sources";
    vector<string> sources;

    if (exists(directorypath) && is_directory(directorypath)){
      for(const auto& entry : directory_iterator(directorypath)) {
        if (entry.path().extension() == ".txt") {
          sources.push_back(entry.path().filename().string());
        }
      }
    } else {
      cerr << "Directory not found." << endl;
      return 1;
    }
    
    // if not .txt files found error prompt
    if (sources.empty()) {
      cerr << "There is no files in ./sources/ - Dai Lai" << endl;
      return 1;
    }
    
    // picks a random file
    string filename = sources[rand() % sources.size()];

    game human_game = humansolve(filename);
    game llm_game = llmsolve(human_game);
    human_game.display_comparison(llm_game);

    // prompts until valid response is received.
    do{
    cout << "Wanna try again? (y/n): ";
    cin >> playAgain;
    }while (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' && playAgain != 'N');
  } while (playAgain == 'y' || playAgain == 'Y');

  // exit title 
  cout << "Thanks for playing Langdale. Adios!" << endl;
  return 0;
}

// ^ FARUK
