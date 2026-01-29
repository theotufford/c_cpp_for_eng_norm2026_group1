#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
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

struct vector_2d {
  int x, y;
  friend bool operator ==(vector_2d const& vec_a, vector_2d const& vec_b){
    return vec_a.x == vec_b.x && vec_a.y == vec_b.y;
  };
  vector_2d() : x(0), y(0) {}
  vector_2d(int x, int y) : x(x), y(y) {}
};



std::vector<vector_2d> make_point_model(int input_coords[][2],
                                        int point_count) {
  std::vector<vector_2d> points;
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
  std::vector<vector_2d> model; // vector of 2d vectors relative to its
                                // transform that make up the body of the obj
  int direction;                // cardinal direction
  int move_increment;
  vector_2d current_position;
  vector_2d prev_position;
  Game &owning_game;
  void queue_render();
  void collsion_check();
  void handle_collision(gameObj * colliding_object);
  void abs_transform(vector_2d coords);
  void delta_transform(vector_2d delta);
  gameObj(Game &owning_game);
};

class playerObj : public gameObj {
public:
  void handle_player_input(input_event key_event);
  playerObj(Game &owning_game);
};

class asciiCanvas {
private:
  const char _populated_character;
  const char _background_character;
  int **_canvas;

public:
  const int height, width;
  void render();
  std::vector<vector_2d> render_queue;
  std::vector<vector_2d> render_dequeue;
  void fill(int x_coord, int y_coord);
  void clear(int x_coord, int y_coord);
  std::stringstream text_footer;
  asciiCanvas(                                      //
      int height,                                   //
      int width,                                    //
      char populated_character,                     //
      char background_character)                    //
      : height(height), width(width),               //
        _populated_character(populated_character),  //
        _background_character(background_character) //
  {                                                 //
    _canvas = (int **)malloc(height * sizeof(int **));
    for (int y_ind = 0; y_ind < height; ++y_ind) {
      _canvas[y_ind] = (int *)malloc(width * sizeof(int));
      for (int x_ind = 0; x_ind < width; ++x_ind) {
        _canvas[y_ind][x_ind] = 0;
      }
    }
  }
};

class Game {
public:
  asciiCanvas *canvas_instance;
  std::vector<gameObj *> objects; // interesting idea would be to order this by
                                  // position to optimize
};

gameObj::gameObj(Game &owning_game) :owning_game(owning_game){
  owning_game.objects.push_back(this);
  queue_render();
}

playerObj::playerObj(Game &owning_game) : gameObj(owning_game) {
};

void gameObj::handle_collision(gameObj * colliding_object){
  system("clear");
  std::cout << "center collision !!!"<< std::endl;
  std::exit(EXIT_SUCCESS);
}

void gameObj::collsion_check() {
  for (gameObj *other_obj_ptr : owning_game.objects) {
    if (this == other_obj_ptr) {
      continue; // other objects not this one
    }
    if (current_position == other_obj_ptr->current_position) {
      other_obj_ptr->handle_collision(this);
      handle_collision(other_obj_ptr);
    }
  }
}


void gameObj::queue_render() {
  asciiCanvas *active_canvas = owning_game.canvas_instance;
  active_canvas->text_footer.str("");
  for (vector_2d model_vector : model) {
    vector_2d abs_position;
    abs_position.x = current_position.x + model_vector.x;
    abs_position.y = current_position.y + model_vector.y;
    active_canvas->render_queue.push_back(abs_position);
  }
  if (current_position == prev_position) {
    return;
  }
  active_canvas->text_footer << "cached coords: ";
  for (vector_2d model_vector : model) {
    vector_2d abs_position;
    abs_position.x = prev_position.x + model_vector.x;
    abs_position.y = prev_position.y + model_vector.y;
    active_canvas->text_footer << "(" << abs_position.x << "," << abs_position.y
                               << ") ";
    active_canvas->render_dequeue.push_back(abs_position);
  }
}

void gameObj::abs_transform(vector_2d coords) {
  prev_position = current_position;
  current_position = coords;
  queue_render();
}

void gameObj::delta_transform(vector_2d delta) {
  prev_position = current_position;
  current_position.x += delta.x;
  current_position.y += delta.y;
  queue_render();
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
    std::exit(EXIT_SUCCESS);
  }
  delta_transform(transform_vector);
  return;
}
// re-render the ascii canvas building a string stream to represent
// the entire display and then printing it all at once assembled
void asciiCanvas::render() {
  // set stale points in dequeue vector to background
  while (!render_dequeue.empty()) {
    vector_2d vector = render_dequeue.back();
    if (vector.x < width && vector.x > 0 && vector.y < height && vector.y > 0) {
      _canvas[vector.y][vector.x] = 0;
    }
    render_dequeue.pop_back();
  } // activate or reactivate living points in render queue
  while (!render_queue.empty()) {
    // I havent been able to figure out hash maps
    // for custom structs. so its actually more efficient
    // to re-render everything than it is to search for
    // duplicate renders and remove them.
    // this would not be the case if I understood hash maps. but
    // as of now I would have to iteratively search the entire
    // point model to see if there are duplicates.
    vector_2d &vector = render_queue.back();
    if (vector.x < width && vector.x > 0 && vector.y < height && vector.y > 0) {
      _canvas[vector.y][vector.x] = 1;
    }
    render_queue.pop_back();
  }
  system("clear");                 // clear screen
  std::stringstream screen_output; // actual rendered output sstream
  for (int y_ind = 0; y_ind < height; ++y_ind) {
    for (int x_ind = 0; x_ind < width; ++x_ind) {
      // scan entire canvas for 1s
      // insert a populated char when found, otherwise insert bg char
      if (_canvas[y_ind][x_ind] == 1) {
        screen_output << _populated_character;
        continue;
      }
      screen_output << _background_character;
    }
    screen_output << "\n";
  }
  std::cout << screen_output.str() << text_footer.str() << "\ny to quit"
            << std::endl;
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
  asciiCanvas canvas(canvas_height, canvas_width, canvas_populated,
                     canvas_background);
  generic_game.canvas_instance = &canvas;

  // ---- player setup ----
  playerObj guy(generic_game);
  vector_2d player_spawn_point = vector_2d(canvas_width / 2, canvas_height / 2);
  guy.current_position = player_spawn_point;
  int guy_model_points[][2] = {{0, 1},  {1, 0},  {1, 1},  {-1, 0},
                               {0, -1}, {-1, 1}, {1, -1}, {-1, -1}};
  guy.model = make_point_model(guy_model_points, std::size(guy_model_points));

  // ---- game obstacle setup ----
  gameObj brick_wall(generic_game);
  vector_2d wall_center = vector_2d(10,10);
  brick_wall.current_position = wall_center;
  int wall_model_points[][2]={
    {0,0},{0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7},{0,8},{0,9},{0,10},{0,11},{0,12},{0,13},{0,14},{0,15},
    {1,0},{1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},{1,11},{1,12},{1,13},{1,14},{1,15},
    {2,0},{2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8},{2,9},{2,10},{2,11},{2,12},{2,13},{2,14},{2,15},
    {3,0},{3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8},{3,9},{3,10},{3,11},{3,12},{3,13},{3,14},{3,15},
    {4,0},{4,1},{4,2},{4,3},{4,4},{4,5},{4,6},{4,7},{4,8},{4,9},{4,10},{4,11},{4,12},{4,13},{4,14},{4,15},
    {5,0},{5,1},{5,2},{5,3},{5,4},{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},{5,11},{5,12},{5,13},{5,14},{5,15},
    {6,0},{6,1},{6,2},{6,3},{6,4},{6,5},{6,6},{6,7},{6,8},{6,9},{6,10},{6,11},{6,12},{6,13},{6,14},{6,15},
    {7,0},{7,1},{7,2},{7,3},{7,4},{7,5},{7,6},{7,7},{7,8},{7,9},{7,10},{7,11},{7,12},{7,13},{7,14},{7,15},
    {8,0},{8,1},{8,2},{8,3},{8,4},{8,5},{8,6},{8,7},{8,8},{8,9},{8,10},{8,11},{8,12},{8,13},{8,14},{8,15},
    {9,0},{9,1},{9,2},{9,3},{9,4},{9,5},{9,6},{9,7},{9,8},{9,9},{9,10},{9,11},{9,12},{9,13},{9,14},{9,15},
  };

  brick_wall.model = make_point_model(wall_model_points, std::size( wall_model_points ));

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

  brick_wall.queue_render();
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
    canvas.render();
  }

  // cleanup
  close(file_dscrptr);
  return 0;
}
