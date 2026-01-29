#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <iterator>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unistd.h>
#include <vector>

struct point_2d {
  int x, y;
  point_2d() : x(0), y(0) {}
};

std::vector<point_2d> make_point_model(int input_points[][2], int point_count) {
  std::vector<point_2d> points;
  // insane silent bug just fixed where not initializing point index to 0
  // caused the loop to quit instantly and silently
  // broke everything very quietly
  for (int point_index = 0; point_index < point_count; point_index++) {
    point_2d point_tmp;
    point_tmp.x = input_points[point_index][0];
    point_tmp.y = input_points[point_index][1];
    points.push_back(point_tmp);
  }
  return points;
}

class Game;

class gameObj {
public:
  std::vector<point_2d> model; // vector of points relative to its transform
                               // that make up the body of the obj
  int direction; // cardinal direction
  point_2d current_position;
  Game &owning_game;
  gameObj(Game &owning_game) : owning_game(owning_game) {}
};

class actorObj : public gameObj {
public:
  void handle_player_input(input_event key_event);
  int move_increment;
  actorObj(Game &owning_game, int move_increment);
};

class asciiCanvas {
private:
  const char _populated_character;
  const char _background_character;
  int **_canvas;

public:
  const int height, width;
  void render();
  std::vector<point_2d> render_queue;
  std::vector<point_2d> render_dequeue;
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
  std::vector<gameObj *> objects;
};

actorObj::actorObj(Game &owning_game, int move_increment)
    : gameObj(owning_game), move_increment(move_increment) {
  owning_game.objects.push_back(this);
};

enum keybinds {
  LEFT_BIND = KEY_A,
  RIGHT_BIND = KEY_D,
  UP_BIND = KEY_W,
  DOWN_BIND = KEY_S,
  EXIT_BIND = KEY_Y,
};

// generate direction vector from input
void actorObj::handle_player_input(input_event key_event) {
  asciiCanvas *active_canvas = owning_game.canvas_instance;
  for (point_2d model_point : model) {
    point_2d abs_position;
    abs_position.x = current_position.x + model_point.x;
    abs_position.y = current_position.y + model_point.y;
    active_canvas->render_dequeue.push_back(abs_position);
  }
  switch (key_event.code) {
  case RIGHT_BIND:
    current_position.x += move_increment;
    break;
  case LEFT_BIND:
    current_position.x -= move_increment;
    break;
  case UP_BIND:
    current_position.y -= move_increment;
    break;
  case DOWN_BIND:
    current_position.y += move_increment;
    break;
  case EXIT_BIND:
    std::exit(EXIT_SUCCESS);
  }
  active_canvas->text_footer.str("");
  for (point_2d model_point : model) {
    point_2d abs_position;
    abs_position.x = current_position.x + model_point.x;
    abs_position.y = current_position.y + model_point.y;
    active_canvas->render_queue.push_back(abs_position);
  }
  return;
}
void asciiCanvas::render() {
  while (!render_dequeue.empty()) {
    point_2d point = render_dequeue.back();
    if (point.x < width && point.x > 0 && point.y < height && point.y > 0) {
      _canvas[point.y][point.x] = 0;
    }
    render_dequeue.pop_back();
  }
  while (!render_queue.empty()) {
    point_2d &point = render_queue.back();
    if (point.x < width && point.x > 0 && point.y < height && point.y > 0) {
      _canvas[point.y][point.x] = 1;
    }
    render_queue.pop_back();
  }
  system("clear");
  std::stringstream screen_output;
  int true_index = 0;
  for (int y_ind = 0; y_ind < height; ++y_ind) {
    for (int x_ind = 0; x_ind < width; ++x_ind) {
      if (_canvas[y_ind][x_ind] == 1) {
        screen_output << _populated_character;
        continue;
      }
      screen_output << _background_character;
    }
    screen_output << "\n";
  }
  std::cout << screen_output.str() << text_footer.str() << std::endl;
}

int main() {
  struct input_event event;
  Game generic_game;

  int canvas_height = 30;
  int canvas_width = 80;
  char canvas_background = '.';
  char canvas_populated = '#';
  asciiCanvas canvas(canvas_height, canvas_width, canvas_populated,
                     canvas_background);
  int move_increment = 1;
  actorObj guy(generic_game, move_increment);

  int guy_model_points[][2] = {{0, 0},  {0, 1},  {1, 0},  {1, 1},  {-1, 0},
                               {0, -1}, {-1, 1}, {1, -1}, {-1, -1}};

  guy.model = make_point_model(guy_model_points, std::size(guy_model_points));
  generic_game.canvas_instance = &canvas;

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
    canvas.render();
  }

  // cleanup
  close(file_dscrptr);
  return 0;
}
