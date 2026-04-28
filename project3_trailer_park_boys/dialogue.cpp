#include "dialogue.hpp"
#include <cctype>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

void DialogueBranch::progress_dialogue() {}

// should basically composite ascii_background and the current_dialogue
// into a single string of text and then display it to the screen
string DialogueBranch::render() {}
DialogueBranch DialogueBranch::get_choice() {}

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
  enum { EMPTY, IN_PROMPT, IN_TAGS, IN_DIALOGUE, IN_OPTIONS };
  int data_destination = EMPTY;

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
    bool add_to_temp = false;
    char char_to_check = *string_pos;
    if (isalpha(char_to_check)) {
      add_to_temp = true;
    } else {
      switch (char_to_check) {
      case ' ': // ignore whitespace when outside of string
        if (reading_string) {
          add_to_temp = true;
        }
        break;
      case '\n': // ignore newline outside of string
        if (reading_string) {
          add_to_temp = true;
        }
        break;
      case '\\': // respect escaped specials
        ++string_pos;
        char_to_check = *string_pos;
        add_to_temp = true;
        break;
      case '"': { // handle quotes
        // quotes toggle string reading
        bool is_opening_quote = !reading_string;
        if (is_opening_quote) {
          temp = "";
          // start reading string
          reading_string = true;
          break;
        } else { // closing quote, handle string
          reading_string = false;
          break;
        }
      }
      case '[':
        if (reading_string) {
          break;
        }
        if (temp == "prompt") {
          data_destination = IN_PROMPT;
        }
        if (temp == "branch_tags") {
          data_destination = IN_TAGS;
        }
        if (temp == "dialogue") {
          data_destination = IN_DIALOGUE;
        }
        if (temp == "options") {
          data_destination = IN_OPTIONS;
        }
        temp = "";
        break;
      case ']':
        if (reading_string) {
          break;
        }
        data_destination = EMPTY;
        break;
      case '{':
        if (reading_string) {
          break;
        }
      case '}': 
        if (reading_string) {
          break;
        }
      }
      // end of switch statement
    }
  }
}
