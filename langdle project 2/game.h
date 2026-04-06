#include <cstdlib>
#include <exception>
#include <string>
#include <vector>

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
  int winstate = 0;

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
