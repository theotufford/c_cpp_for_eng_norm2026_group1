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
  int guesscount;
  vector<string> guesslog;
  int winstate;

  public:
  vector<string> puzzle;
  string secret_word;
  int guess(string input_guess);
  enum {
    WRONG,
    GAME_WON,
    GAME_LOST,
    START
  };
  void display_comparison(game other);
  void set_puzzle(string puzzletext);
  game(string source_filename);
  game();
  game newgame_from_puzzle();
  game get_testgame();

};
