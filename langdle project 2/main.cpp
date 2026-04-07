#include "game.cpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::filesystem;

// ran out of time to implement this :(
// use a local AI model to play the game
game llmsolve(string fileneame) {
  game llmgame = game(fileneame);

  // make the AI play

  return llmgame;
}
// ^ THEO

// main gameplay loop
game humansolve(string filename) {

  cout << "file picked: " << filename << endl;

  // explicitly calling the
  // constructor here makes it a
  // bit clearer
  game humangame = game(filename);

  string response;
  for (string given_info : humangame.puzzle) {
    cout << "given_info: " << given_info << "_______" <<  humangame.punct  << endl;
    cin.clear();
    cin >> response;
    switch (humangame.guess(response)) {
    case game::GAME_WON:
      cout << "you win!!" << endl;
      return humangame;
    case game::GAME_LOST:
      cout << "no more guesses" << endl
           << "the secret word was: " << humangame.secret_word << endl;
      break;
    case game::WRONG:
      cout << "wrong guess!" << endl;
      break;
    }
  }
  return humangame;
}

// the main role of this is to repeat the game until the user quits
// should also iterate through all the local text files and randomly
// pick one to build that round of the game from
//
// then compare the two games and repeat
int main() {
  srand(time(nullptr)); // seeds rand() so same file isnt opened everytime

  char playAgain; // tracks whether user want to play again
  do {
    // display general game title and information
    cout << "====================================" << endl;
    cout << "              LANGDLE" << endl;
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

    if (exists(directorypath) && is_directory(directorypath)) {
      for (const auto &entry : directory_iterator(directorypath)) {
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

    // game llm_game = llmsolve(filename);
    // human_game.display_comparison(llm_game);

    // prompts until valid response is received.
    do {
      cout << "Wanna try again? (y/n): ";
      cin >> playAgain;
    } while (playAgain != 'y' && playAgain != 'Y' && playAgain != 'n' &&
             playAgain != 'N');
  } while (playAgain == 'y' || playAgain == 'Y');

  // exit title
  cout << "Thanks for playing Langdle. Adios!" << endl;
  return 0;
}

// ^ FARUK
