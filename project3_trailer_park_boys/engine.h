#include "utils.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#define MAP_WIDTH 31
#define MAP_HEIGHT 20

using namespace std;

// DialogueBranch class
// holds dialogue progression
class DialogueBranch {
public:
  static vector<DialogueBranch *> all_branches;
  vector<string> dialogue_content;
  string prompt;
  vector<string> tags;              // never used
  vector<DialogueBranch *> options; // child options
  friend DialogueBranch *make_dialogue_tree(string::iterator &string_pos,
                                            string::iterator &backstop);
};
// depreciated
class charSprite {
public:
  Rect bounding_box;
  char *data;
  charSprite(vec2d &center, char *data);
};

class gameObject;
class Environment;
class Player;
class Interactable;
class Pawn;
class NPC;

// game map class that holds game state info, and a method to extract it
// re-implemented from elis original class
class gameMap {
private:
  gameMap(int width, int height);

public:
  const int height;
  const int width;
  vector<gameObject *> live_objects; // current live objects
  Rect map_bound;                    // bounds player
  Player *player;
  bool delete_object(gameObject * obj);
  string render_to_string(); // output
  string message_to_player;  // all of the live objects can mutate this
                             // to give an output message to the player without
                             // the full interaction cycle

  // creates new game map from file, initializes objects etc.
  friend gameMap generate_map_from_file(string filename);
};

// this is like elis mapObject except abstract
class gameObject {
public:
  vec2d position; // position
  char sprite;    // different term for elis "rep"
  string name;    // shows up in dialogue
  gameMap *owning_map;
  virtual string
  object_type() = 0; // this is a really bad use of polymorphism
                     // because this is the only function that makes
                     // this polymorphic but I always use dynamic_cast anyways
                     // so its never actually called but if I remove
                     // it dynamic_cast stops working
  gameObject(gameMap &owning_map, vec2d position, char sprite, string name);
};

// https://www.geeksforgeeks.org/cpp/virtual-base-class-in-c/
// I dont really fully understand why these need
// to be virtual but it boils down to the fact that
// NPC points back to definitional parents of gameObject
// if this isnt the case
// ultimately it doesnt effect anything else to my knowledge
// so its sort of fine
class Environment : virtual public gameObject {
public:
  Environment(gameMap &owning_map, vec2d position, char sprite);
  string object_type();
};

// pawns are game objects that can move and detect things
class Pawn : virtual public gameObject {

public:
  const Rect detection_hitbox = {
      6, 6, gameObject::position}; // fixed width default detection hitbox
  vec2d direction = {1, 0};        // used for forward call
  bool move(const vec2d &move_increment); // just straight up move
  bool forward(int distance);             // forward in direction
  string object_type();
  void detection_check(); // this always calls detection_response
                          // which is implemented on a class by class
                          // basis hence the reason it has a void return
  Pawn(gameMap &owning_map, vec2d position, char sprite, string name);

private:
  virtual void detection_response(gameObject *detected_object) = 0;
};

// anything that owns a dialogue tree and that you can interact with
class Interactable : virtual public gameObject {
private:
  DialogueBranch *dialogue_tree =
      nullptr; // tracks the head of the dialogue tree

public:
  stringstream interaction_ascii;
  DialogueBranch *current_dialogue_branch = nullptr;
  string object_type();
  void give_dialogue_tree_from_file(string filename);
  Interactable(gameMap &owning_map, vec2d position, char sprite, string name);
};

// the player
class Player : public Pawn {
private:
  void detection_response(gameObject *detected_object);
  static constexpr char playersprite = '@';

public:
  int grills_stolen;
  string object_type();
  Player(gameMap &owning_map, vec2d position);
};

class NPC : public Interactable, public Pawn {
private:
  void detection_response(gameObject *detected_object);

public:
  bool is_hostile = false;
  int patrol_length = 3;
  void patrol_move();
  string object_type();
  NPC(gameMap &owning_map, vec2d position, char sprite, string name);
};
