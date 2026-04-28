#include <map>
#include <string>
#include <vector>

using namespace std;
class DialogueBranch {
private:
  string ascii_background;
  vector<string> dialogue_content;

public:
  void progress_dialogue();
  string render();
  string option_prompt;
  map<string, DialogueBranch *> choices;
  void make_choice(int choice);
  friend DialogueBranch *make_dialogue_tree(string::iterator string_pos,
                                            string::iterator backstop);
};
