#include <cstdlib>
#include <exception>
#include <string>
#include <vector>

using namespace std;
#define GUESSMAX 5
  enum {
    WRONG,
    GAME_WON,
    GAME_LOST,
    START
  };

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
  char punct;
  int guess(string input_guess);
  void display_comparison(game other);
  void set_puzzle(string puzzletext);
  game(string source_filename);
  game();

};
