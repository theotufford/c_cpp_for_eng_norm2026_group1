#include "dialogue.hpp"
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

void DialogueBranch::progress_dialogue() {}

// should basically composite ascii_background and the current_dialogue
// into a single string of text and then display it to the screen
string DialogueBranch::render() {}
void DialogueBranch::make_choice(int choice) {}

// Make top level new dialogue branch with json string string
// heap allocate a new DialogueBranch
// check for "dialogue option", find first "["
// find next '\"'
// read until ',' remove closing '\"',
// repeat until "]" found append each string to dialogue content vector
// read next quote-enclosed string as dialogue option
// recursive call this function on string after next "{"
// repeat recursive call until "}" found
// return DialogueBranch

DialogueBranch *make_dialogue_tree(string::iterator string_pos,
                                   string::iterator backstop) {

  auto current_branch = new DialogueBranch();
  bool reading_string = false;

  string temp = "";
  int openBrackCounter = 1;
  while (true) {

    if (string_pos == backstop) {
      string errmsg =
          "ERROR! bad json formatting, file didnt end with a } backstop was: ";
      errmsg += *backstop;
      throw errmsg;
    }
    string_pos++; // start at given +1 because given 0 will always
                  // be a redundant open bracket
    char char_to_check = *string_pos;
    if (isalpha(char_to_check)) {
      temp += char_to_check;
    } else {
      switch (char_to_check) {

        // ignore whitespace
      case ' ':
        if (reading_string) {
          temp += char_to_check;
        }
        break;
      case '\n':
        if (reading_string) {
          temp += char_to_check;
        }
        break;

      // respect escaped specials
      case '\\':
        ++string_pos;
        temp += *string_pos;
      // handle quotes
      case '"': {
        // quotes toggle string reading
        bool is_opening_quote = !reading_string;

        if (is_opening_quote) {
          // clear the string string
          temp = "";
          // start reading string
          reading_string = true;
          break;
        } else { // closing quote, handle string
          reading_string = false;
          string_pos++;
          while (*string_pos == ' ' || *string_pos == '\n') {
            string_pos++;
          }
          char &nextchar = *string_pos;
          // "close-quote comma" implies a list of strings
          // which should only show up in dialogue
          if (nextchar == ',' || nextchar == ']') {
            current_branch->dialogue_content.push_back(temp);
          }
          break;
        }
      }
      // handle new dialogue option when { seen
      // via recursive call and insertion in the dialogue
      // options map
      case '{':
        if (reading_string) {
          break;
        }
        openBrackCounter++;
        current_branch->choices.insert(
            {temp, make_dialogue_tree(string_pos, backstop)});
        break;
      case '}':
        if (reading_string) {
          break;
        }
        openBrackCounter--;
        if (openBrackCounter == 0) {
          return current_branch;
        }
      }
    }
  }
}
