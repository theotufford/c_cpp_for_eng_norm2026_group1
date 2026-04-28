#include "dialogue.cpp"
#include <string>

string teststring =
    "{ \"dialogue\" :[ \n \"this is some dialogue\"\n], \"this is an "
    "option\":{ \"dialogue\":[ \"this is subdialogue\", \"this is even more "
    "dialogue\"] }, \"this is another option\" : { \"dialogue\" :[ \"this is "
    "its dialogue\"] } }";

int main() {
  auto &dia_tree_base = *make_dialogue_tree(teststring.begin(), teststring.end());

  return 0;
}
