#include <map>
#include <string>
#include <vector>

using namespace std;

struct DialogueTag {
  string name; 
  string data; 
};

class DialogueBranch {
private:
  string ascii_background;
  vector<string> dialogue_content;

public:
  vector<DialogueTag> tags;
  vector<DialogueBranch *> choices;
  string prompt;

  void progress_dialogue();
  string render();
  DialogueBranch get_choice();
  friend DialogueBranch *make_dialogue_tree(string::iterator string_pos,
                                            string::iterator backstop);
};
