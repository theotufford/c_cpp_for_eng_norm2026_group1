#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <functional>
#include <iostream>
#include <iterator>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <unordered_set>
#include <vector>

using namespace std;

class gameObj;

struct vector_2d {
  int x, y;
  int color_value;
  gameObj *collision_owner;
  friend bool operator==(vector_2d const &vec_a, vector_2d const &vec_b) {
    return vec_a.x == vec_b.x && vec_a.y == vec_b.y;
  };
  friend vector_2d operator+(vector_2d const &vec_a, vector_2d const &vec_b) {
    vector_2d outvec;
    outvec.x = vec_a.x + vec_b.x;
    outvec.y = vec_a.y + vec_b.y;
    return outvec;
  }
  vector_2d() : x(0), y(0), color_value(1) {}
  vector_2d(int x, int y) : x(x), y(y), color_value(1) {}
};
namespace std {
template <> struct hash<vector_2d> {
  // sets the () operator on vector_2d to return a hash
  size_t operator()(const vector_2d &vec) const {
    size_t x_hash = hash<int>{}(vec.x);
    size_t y_hash = hash<int>{}(vec.y);
    return (x_hash << 16) ^ y_hash;
  }
  // bit shift to avoid commutative collision
  // i feel like there is a collision lurking here
  // but for now testing indicates that with a large
  // enough bitshift the non-commutative destruction
  // of information will make the hashing function
  // non invertable
};
} // namespace std
vector<vector_2d> make_point_model(int input_coords[][2], int point_count) {
  vector<vector_2d> points;
  // insane silent bug just fixed where not initializing vector index to 0
  // caused the loop to quit instantly and silently
  // broke everything very quietly
  for (int coord_index = 0; coord_index < point_count; coord_index++) {
    vector_2d point_tmp;
    point_tmp.x = input_coords[coord_index][0];
    point_tmp.y = input_coords[coord_index][1];
    points.push_back(point_tmp);
  }
  return points;
}

class Game;

class gameObj {
public:
  Game &owning_game;
  vector<vector_2d> model; // vector of 2d vectors relative to its
                           // transform that make up the body of the obj
  vector_2d current_position;
  void handle_collision(gameObj *colliding_object);
  gameObj(Game &owning_game) : owning_game(owning_game) {}
};
class environment_object : public gameObj {
public:
  environment_object(Game &owning_game, int *points[2], vector_2d position);
};

// game object that can move
class actorObj : public gameObj {
public:
  vector_2d prev_position;
  void abs_transform(vector_2d coords);
  void delta_transform(vector_2d delta);
  actorObj(Game &owning_game);
};

// actor object that consumes player input
class playerObj : public actorObj {
public:
  void handle_player_input(input_event key_event);
  playerObj(Game &owning_game);
};

class asciiCanvas {
private:
  const char _populated_character;
  const char _background_character;
  const Game &_owning_game;

public:
  bool render_needed = true;
  int **int_map;
  const int height, width;
  void render();
  unordered_set<vector_2d> rendered_points;
  unordered_set<vector_2d> always_rendered; // environemnt objects
  void fill(int x_coord, int y_coord);
  void clear(int x_coord, int y_coord);
  stringstream text_footer;
  asciiCanvas(                                                   //
      Game &owning_game,                                         //
      int height,                                                //
      int width,                                                 //
      char populated_character,                                  //
      char background_character)                                 //
      : _owning_game(owning_game), height(height), width(width), //
        _populated_character(populated_character),               //
        _background_character(background_character)              //
  {                                                              //
    int_map = (int **)malloc(height * sizeof(int **));
    for (int y_ind = 0; y_ind < height; ++y_ind) {
      int_map[y_ind] = (int *)malloc(width * sizeof(int));
      for (int x_ind = 0; x_ind < width; ++x_ind) {
        int_map[y_ind][x_ind] = 0;
      }
    }
  }
};

class Game {
public:
  asciiCanvas *canvas_instance;
  actorObj *player_ptr;
  vector<actorObj *> actor_ptrs;
  vector<environment_object *> env_obj_ptrs;
  // eventually start end goal scoring etc
};
void own_point_model(gameObj *collision_owner) {
  for (vector_2d model_point : collision_owner->model) {
    model_point.collision_owner = collision_owner;
  }
}

actorObj::actorObj(Game &owning_game) : gameObj(owning_game) {
  owning_game.actor_ptrs.push_back(this);
  prev_position = vector_2d(0, 0);
}
environment_object::environment_object(Game &owning_game, vector_2d position, vector<vector_2d> point_model)
    : gameObj(owning_game){
  model = point_model;
  for (vector_2d model_point : model) {
    model_point = model_point + current_position;
  }
  owning_game.env_obj_ptrs.push_back(this);
}
playerObj::playerObj(Game &owning_game) : actorObj(owning_game) {
  owning_game.player_ptr = this;
}

void gameObj::handle_collision(gameObj *colliding_object) {
  system("clear");
  cout << "center collision !!!" << std::endl;
  exit(EXIT_SUCCESS);
}

void actorObj::abs_transform(vector_2d coords) {
  prev_position = current_position;
  current_position = coords;
  owning_game.canvas_instance->render_needed = true;
}

void actorObj::delta_transform(vector_2d delta) {
  prev_position = current_position;
  current_position.x += delta.x;
  current_position.y += delta.y;
  owning_game.canvas_instance->render_needed = true;
}

enum keybinds {
  LEFT_BIND = KEY_A,
  RIGHT_BIND = KEY_D,
  UP_BIND = KEY_W,
  DOWN_BIND = KEY_S,
  EXIT_BIND = KEY_Y,
};

// generate direction vector from input
void playerObj::handle_player_input(input_event key_event) {
  vector_2d transform_vector;
  switch (key_event.code) {
  case RIGHT_BIND:
    transform_vector = vector_2d(1, 0);
    break;
  case LEFT_BIND:
    transform_vector = vector_2d(-1, 0);
    break;
  case UP_BIND:
    transform_vector = vector_2d(0, -1);
    break;
  case DOWN_BIND:
    transform_vector = vector_2d(0, 1);
    break;
  case EXIT_BIND:
    exit(EXIT_SUCCESS);
  }
  delta_transform(transform_vector);
  return;
}
// re-render the ascii canvas building a string stream to represent
// the entire display and then printing it all at once assembled
void asciiCanvas::render() {
  for (environment_object *env_obj_ptr : _owning_game.env_obj_ptrs) {
    // collision between environement objects is ignored
    // and position is static so no calculations are needed
    for (vector_2d model_vector : env_obj_ptr->model) {
      int_map[model_vector.y][model_vector.x] = model_vector.color_value;
    }
  }
  for (actorObj *actor_ptr : _owning_game.actor_ptrs) {
    // iterate through points in object
    for (vector_2d model_vector : actor_ptr->model) {
      vector_2d abs_position;
      vector_2d prev_abs_position;
      abs_position.x = actor_ptr->current_position.x + model_vector.x;
      abs_position.y = actor_ptr->current_position.y + model_vector.y;
      prev_abs_position.x = actor_ptr->prev_position.x + model_vector.x;
      prev_abs_position.y = actor_ptr->prev_position.y + model_vector.y;
      int_map[prev_abs_position.y][prev_abs_position.x] = 0;
      if (rendered_points.contains(abs_position)) { // collision detected
        text_footer << "colliding coords: " << abs_position.x << abs_position.y
                    << "\t";
        // get colliding gameObj
        gameObj colliding_entity =
            *rendered_points.find(abs_position)->collision_owner;
        // calls the collision handler of this game obj
        actor_ptr->handle_collision(&colliding_entity);
        // calls the collision handler of the other game obj
        colliding_entity.handle_collision(actor_ptr);
        continue;
      }
      int_map[abs_position.y][abs_position.x] = model_vector.color_value;
      rendered_points.insert(abs_position);
    }
  }
  system("clear");                   // clear screen
  rendered_points = always_rendered; // clear actor render history
  stringstream screen_output;        // actual rendered output sstream
  for (int y_ind = 0; y_ind < height; ++y_ind) {
    for (int x_ind = 0; x_ind < width; ++x_ind) {
      // scan entire canvas for 1s
      // insert a populated char when found, otherwise insert bg char
      if (int_map[y_ind][x_ind] == 1) {
        screen_output << _populated_character;
        continue;
      }
      screen_output << _background_character;
    }
    screen_output << "\n";
  }
  cout << screen_output.str() << text_footer.str() << "\ny to quit" << endl;
  render_needed = false;
  return;
}

int main() {
  // ---- game setup ----
  struct input_event event;
  Game generic_game;

  // ---- canvas setup ----
  int canvas_height = 40;
  int canvas_width = 80;
  char canvas_background = '.';
  char canvas_populated = '@';
  asciiCanvas canvas(generic_game, canvas_height, canvas_width,
                     canvas_populated, canvas_background);
  generic_game.canvas_instance = &canvas;

  // ---- player setup ----
  playerObj guy(generic_game);
  vector_2d player_spawn_point = vector_2d(canvas_width / 2, canvas_height / 2);
  guy.current_position = player_spawn_point;
  int guy_model_points[][2] = {{0, 1},  {1, 0},  {1, 1},  {-1, 0},
                               {0, -1}, {-1, 1}, {1, -1}, {-1, -1}};
  guy.model = make_point_model(guy_model_points, size(guy_model_points), &guy);

  // ---- game obstacle setup ----
  int wall_model_points[][2] = {
      // i probably should make a function to connect nodes with edges
      // but for now the 2d array of doom
      {0, 0}, {0, 1}, {0, 2},  {0, 3},  {0, 4},  {0, 5},  {0, 6},  {0, 7},
      {0, 8}, {0, 9}, {0, 10}, {0, 11}, {0, 12}, {0, 13}, {0, 14}, {0, 15},
      {1, 0}, {1, 1}, {1, 2},  {1, 3},  {1, 4},  {1, 5},  {1, 6},  {1, 7},
      {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}, {1, 13}, {1, 14}, {1, 15},
      {2, 0}, {2, 1}, {2, 2},  {2, 3},  {2, 4},  {2, 5},  {2, 6},  {2, 7},
      {2, 8}, {2, 9}, {2, 10}, {2, 11}, {2, 12}, {2, 13}, {2, 14}, {2, 15},
      {3, 0}, {3, 1}, {3, 2},  {3, 3},  {3, 4},  {3, 5},  {3, 6},  {3, 7},
      {3, 8}, {3, 9}, {3, 10}, {3, 11}, {3, 12}, {3, 13}, {3, 14}, {3, 15},
      {4, 0}, {4, 1}, {4, 2},  {4, 3},  {4, 4},  {4, 5},  {4, 6},  {4, 7},
      {4, 8}, {4, 9}, {4, 10}, {4, 11}, {4, 12}, {4, 13}, {4, 14}, {4, 15},
      {5, 0}, {5, 1}, {5, 2},  {5, 3},  {5, 4},  {5, 5},  {5, 6},  {5, 7},
      {5, 8}, {5, 9}, {5, 10}, {5, 11}, {5, 12}, {5, 13}, {5, 14}, {5, 15},
      {6, 0}, {6, 1}, {6, 2},  {6, 3},  {6, 4},  {6, 5},  {6, 6},  {6, 7},
      {6, 8}, {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14}, {6, 15},
      {7, 0}, {7, 1}, {7, 2},  {7, 3},  {7, 4},  {7, 5},  {7, 6},  {7, 7},
      {7, 8}, {7, 9}, {7, 10}, {7, 11}, {7, 12}, {7, 13}, {7, 14}, {7, 15},
      {8, 0}, {8, 1}, {8, 2},  {8, 3},  {8, 4},  {8, 5},  {8, 6},  {8, 7},
      {8, 8}, {8, 9}, {8, 10}, {8, 11}, {8, 12}, {8, 13}, {8, 14}, {8, 15},
      {9, 0}, {9, 1}, {9, 2},  {9, 3},  {9, 4},  {9, 5},  {9, 6},  {9, 7},
      {9, 8}, {9, 9}, {9, 10}, {9, 11}, {9, 12}, {9, 13}, {9, 14}, {9, 15},
  };
  environment_object brick_wall(generic_game);
  // this is all weird because of the sizing restrictions and stuff
  brick_wall.model =
      make_point_model(wall_model_points, size(wall_model_points), &brick_wall);
  vector_2d wall_center = vector_2d(10, 10);
  brick_wall.current_position = wall_center;

  // ---- input setup ----
  // TODO get OS input to decide library to use

  // this is a buffer to read input from your keyboard via file_dscrptr
  // because in POSIX compliant machines (mac linux) I/O is handled by
  // writing to a region of memory that programs interface with as if
  // it were a file. which is pretty cool lol.
  const char *dev = "/dev/input/event2";

  // file descriptors are an index in a table of every file
  // linux currently has open and are used as basically
  // a common language between all applications that access files

  int file_dscrptr = open(dev, O_RDONLY);
  if (file_dscrptr == -1) {
    perror("Error opening device");
    return 1;
  }

  // ---- main loop ----

  while (true) {
    bool getting_input =
        0 < read(file_dscrptr, // file descriptor
                 &event,       // give the file data to the input event struct
                 sizeof(struct input_event));
    if (!getting_input) {
      break;
    }
    if (event.type == EV_KEY) {
      guy.handle_player_input(event);
    }
    if (canvas.render_needed) {
      canvas.render();
    }
  }

  // cleanup
  close(file_dscrptr);
  return 0;
}
