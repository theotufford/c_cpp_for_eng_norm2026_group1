// sorry about this my editor sort of just auto imports stuff sometimes and I
// dont know which of these is a waste
#include "engine.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <vector>

// this needs to be done otherwise weird compiler errors happen
vector<DialogueBranch *> DialogueBranch::all_branches = {};

// kind of crappy json parser that creates dialogue trees from a given string
// iterator, file handling is extracted to a different function
DialogueBranch *make_dialogue_tree(string::iterator &string_pos,
                                   string::iterator &backstop) {
  // heap allocate all of the dialogue branches
  // technically this is sort of a memory leak but whatever
  auto current_branch = new DialogueBranch();
  DialogueBranch::all_branches.push_back(current_branch);

  bool reading_string = false; // useful for navigation
  enum { EMPTY, IN_PROMPT, IN_TAGS, IN_DIALOGUE, IN_OPTIONS };
  // read state
  int data_destination = EMPTY;

  string temp = "";

  // iterate with the given string pos iterator
  while (true) {
    if (string_pos == backstop) {
      string errmsg =
          "ERROR! bad json formatting, file didnt end with a } backstop was: ";
      errmsg += *backstop;
      throw errmsg;
    }

    string_pos++; // start at (given position +1) because given position will
                  // always be a redundant open bracket

    char char_to_check = *string_pos;

    // respect escaped quotes by leapfrogging over them so they are
    // never caught
    if (char_to_check == '\\') {
      ++string_pos;
      char_to_check = *string_pos;
      temp += char_to_check;
      continue;
    }
    // quotes toggle string reading
    if (char_to_check == '"') {
      bool is_opening_quote = !reading_string;
      if (is_opening_quote) {
        temp = "";
        // start reading string
        reading_string = true;
      } else { // closing quote, stop reading
        reading_string = false;
      }
      continue;
    }
    if (reading_string) {
      // any characters within quotes are tolerated
      temp += char_to_check;
    } else {
      switch (char_to_check) {
      case ':': // implies we have a key waiting in temp
                // we then use that key to set the data_destination state
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
      // use this case if we have a complete entry or last entry in a list
      case ']':
      case ',':
        switch (data_destination) {
        case IN_TAGS:
          current_branch->tags.push_back(temp);
          break;
        case IN_DIALOGUE:
          current_branch->dialogue_content.push_back(temp);
          break;
        case IN_PROMPT:
          current_branch->prompt = temp;
          break;
        }
        if (char_to_check == ']') {
          // if we are at the end of a list we want to clear the state
          data_destination = EMPTY;
        }
        temp = "";
        break;
      // the only things that should have { wrapping are sub branches
      // when we find one we recursive call this function and then add the
      // returned value to the options of this branch
      case '{':
        if (data_destination == IN_OPTIONS) {
          current_branch->options.push_back(
              make_dialogue_tree(string_pos, backstop));
        }
        break;
      case '}':
        // this would imply we found a ] and then a } which would be the end of
        // the file
        if (data_destination == EMPTY) {
          return current_branch;
        }
      }
    }
  }
}

string gameMap::render_to_string() {
  char out_mat[height][width];
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      out_mat[y][x] = '.';
    }
  }
  for (gameObject *Go : live_objects) {
    auto indVec = Go->position.floor();
    out_mat[indVec.y][indVec.x] = Go->sprite;
  }
  string output;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      output += out_mat[y][x];
      output += " ";
    }
    output += "\n";
  }
  return output;
}

// a whole bunch of constructor chains of doom because I really dont know what
// im doing
gameMap::gameMap(int width, int height)
    : map_bound(Rect(0, 0, height, width)), height(height), width(width) {}
gameObject::gameObject(gameMap &owning_map, vec2d position, char sprite,
                       string name)
    : owning_map(&owning_map), // map to be added to
      position(position),      // position
      sprite(sprite),          // sprite character
      name(name)               // name of game object
{}
Environment::Environment(gameMap &owning_map, vec2d position, char sprite)
    : gameObject(owning_map, position, sprite, "environment") {}
Interactable::Interactable(gameMap &owning_map, vec2d position, char sprite,
                           string name)
    : gameObject(owning_map, position, sprite, name) {
  // interactables are given both a dialogue tree and an ascii avatar
  give_dialogue_tree_from_file("./dialogue/" + name + ".json");
  ifstream ascii_file("./interaction_sprites/" + name + ".txt");
  if (!ascii_file.is_open()) {
    cerr << "failed to open file : "
         << "./BoysAscii/" + name + ".txt" << endl;
  }

  vector<string> lines;
  string tmp;

  while (getline(ascii_file, tmp)) {
    lines.push_back(tmp);
  }

  // this is adapted from faruks original work on the dialogue output
  interaction_ascii << "\033[H\033[2J"
                    << "+------------------+" << lines.at(0) << endl
                    << "|   " << setw(15) << " " << "|" << lines.at(1) << endl
                    << "|   " << setw(15) << left << name << "|" << lines.at(2)
                    << endl
                    << "|   " << setw(15) << " " << "|" << lines.at(3) << endl
                    << "+------------------+" << lines.at(4) << endl;
  for (int i = 5; i < lines.size(); i++) {
    string line = lines.at(i);
    interaction_ascii << setw(20) << " " << line << endl;
  }
  ascii_file.close();
}

Pawn::Pawn(gameMap &owning_map, vec2d position, char sprite, string name)
    : gameObject(owning_map, position, sprite, name) {}

NPC::NPC(gameMap &owning_map, vec2d position, char sprite, string name)
    : gameObject(owning_map, position, sprite, name),
      Interactable(owning_map, position, sprite, name),
      Pawn(owning_map, position, sprite, name) {}

Player::Player(gameMap &owning_map, vec2d position)
    : gameObject(owning_map, position, playersprite, "player"),
      Pawn(owning_map, position, playersprite, "player") {}

string Player::object_type() { return "Player"; }
string Environment::object_type() { return "Environment"; }
string NPC::object_type() { return "NPC"; }
string Interactable::object_type() { return "Interactable"; }
string Pawn::object_type() { return "Pawn"; }

bool gameMap::delete_object(gameObject *obj) {
  // https://www.geeksforgeeks.org/cpp/remove-an-item-from-vector-with-a-certain-value/

  int original_size = live_objects.size();
  live_objects.erase(remove(live_objects.begin(), live_objects.end(), obj),
                     live_objects.end());
  int new_size = live_objects.size();
  bool erase_success = new_size != original_size;
  return erase_success;
}

// adapted from elis mapObject constructor in the original engine
gameMap generate_map_from_file(string filename) {
  ifstream inFile;
  inFile.open("display.txt");
  string activeLine;
  vec2d read_position;

  int height = 0;
  int width = 0;

  while (getline(inFile, activeLine)) {
    int temp_width = 0;
    for (char char_cell : activeLine) {
      temp_width++;
    }
    width = temp_width;
    height++;
  }
  inFile.close();

  inFile.open("display.txt");

  auto newmap = gameMap(width, height);

  while (getline(inFile, activeLine)) {
    for (char char_cell : activeLine) {
      switch (char_cell) {
      case '.':
        break;
      case '#':
      case '%':
      case '|': {
        newmap.live_objects.push_back(
            new Environment(newmap, read_position, char_cell));
        break;
      }
      case '@': {
        newmap.player = new Player(newmap, read_position);
        newmap.live_objects.push_back(newmap.player);
        break;
      }
      case 'r': {
        newmap.live_objects.push_back(
            new NPC(newmap, read_position, 'r', "randy"));
        break;
      }
      case 'J': {
        newmap.live_objects.push_back(
            new NPC(newmap, read_position, 'J', "julian"));
        break;
      }
      case 'B': {
        newmap.live_objects.push_back(
            new NPC(newmap, read_position, 'B', "bubbles"));
        break;
      }
      case 'R': {
        newmap.live_objects.push_back(
            new NPC(newmap, read_position, 'R', "ricky"));
        break;
      }
      case 'g': {
        newmap.live_objects.push_back(
            new Interactable(newmap, read_position, 'g', "grill"));
        break;
      }
      }
      read_position.x++;
    }
    read_position.y++;
    read_position.x = 0;
  }
  return newmap;
}

bool Pawn::move(const vec2d &move_increment) {
  // candidate position
  vec2d target_position = position + move_increment;
  // check if move in bounds
  bool out_of_bounds =
      !target_position.is_within_absolute(owning_map->map_bound);
  if (out_of_bounds) {
    return false;
  }
  for (gameObject *to_check : owning_map->live_objects) {
    // ensure no colliding objects at candidate position
    if (to_check->position == target_position) {
      return false;
    }
  }
  // commit move
  position = position + move_increment;
  return true;
}

bool Pawn::forward(int distance) { return move(direction * distance); }

void Interactable::give_dialogue_tree_from_file(string filename) {
  ifstream file(filename);
  if (!file.is_open()) {
    cerr << "failed to open file : " << filename << endl;
  }
  stringstream file_data;
  file_data << file.rdbuf();
  string jsonString = file_data.str();
  string::iterator dialogue_json_string_begin = jsonString.begin();
  string::iterator dialogue_json_string_end = jsonString.end();
  dialogue_tree =
      make_dialogue_tree(dialogue_json_string_begin, dialogue_json_string_end);
  current_dialogue_branch = dialogue_tree;
}

void Pawn::detection_check() {
  for (gameObject *candidate_ptr : owning_map->live_objects) {
    if (candidate_ptr->position.is_within_absolute(detection_hitbox)) {
      detection_response(candidate_ptr);
    }
  }
}

// detect interactables and prompt interaction, then enter interaction
void Player::detection_response(gameObject *detected_object) {
  // cast the detected object to interactable
  Interactable *interactable_ptr =
      dynamic_cast<Interactable *>(detected_object);
  if (interactable_ptr) {
    DialogueBranch *dialogue = interactable_ptr->current_dialogue_branch;
    // display message about possible interaction
    cout << interactable_ptr->name << ": " << dialogue->prompt << endl
         << "press e to interact" << endl;
    cin.clear();
    char tmp;
    cin >> tmp;
    // check for interaction engagement (input of e)
    if (tmp == 'e') {
      // display the ascii avatar of the interactable
      cout << interactable_ptr->interaction_ascii.str();
      // iterate dialogue tree
      while (true) {
        // go line by line in dialogue
        for (string line : dialogue->dialogue_content) {
          cout << line << endl;
          cout << endl;
        }
        // if this is the last branch in a tree leave dialogue
        if (dialogue->options.size() == 0) {
          break;
        }
        // if not display the options
        for (int i = 0; i < dialogue->options.size(); i++) {
          cout << i << ": " << dialogue->options.at(i)->prompt << endl;
        }
        // prompt dialogue choice
        cout << "choose: ";
        int choice = -1;
        cin.clear();
        while (choice < 0 || choice > dialogue->options.size()) {
          cin >> choice;
        }
        // change current dialogue branch and then repeat
        dialogue = dialogue->options.at(choice);
      }
      if (interactable_ptr->name == "grill") {
        owning_map->delete_object(interactable_ptr);
        grills_stolen++;
      }
      // I meant to implement mutating the interactables dialogue state but that
      // logic is more complicated and im out of time
    }
  }
}

// this was going to make it so when the NPC sees the player it starts chasing
// them but ran out of time
void NPC::detection_response(gameObject *detected_object) {
  if (detected_object->name == "player") {
    // chase player
  }
}

// randomly cycles movement of npcs to patrol around
// also implements the fact that they will bounce off walls they run into
// part of this code is adapted from something faruk wrote, part of it is from
// something sean wrote in a group work session
void NPC::patrol_move() {
  bool obstructed = !forward(1);
  if (obstructed) {
    direction = direction * -1;
    bool trapped = !forward(1);
    if (trapped) {
      bool dead_end = !forward(1);
      direction.rotate_by_rads(M_PI / 2);
      forward(1);
    }
  }
  int rand_between_0_and_len = rand() % patrol_length;
  if (rand_between_0_and_len == 1) {
    direction.rotate_by_rads(M_PI / 2);
  }
}
