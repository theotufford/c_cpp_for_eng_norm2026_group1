#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <memory>
#include <ostream>
#include <tuple>
#include <unistd.h>
#include <vector>


class gameObj {
public:
  std::vector<std::tuple<int, int>> point_model; // vector of point tuples that
                                                 // describe relative position
                                                 // of model points

  int x_position, y_position;           // absolute position
  int prev_x_position, prev_y_position; // previous absolute position
                                        // for render clearing
  gameObj() : x_position(0), y_position(0) {}
};

class actorObj : public gameObj {
public:
  void handle_key(input_event key_event);
  int move_increment;
  actorObj(int move_increment)
      : gameObj(), move_increment(move_increment) {
          // attach to canvas probably
        };
};

class asciiCanvas {
private:
  const char _populated_character;
  const char _background_character;
  char *_canvas;

public:
  const int height, width;
  void render();
  std::vector<gameObj *> objects;
  void toggleCoord(int x_coord, int y_coord);
  asciiCanvas(                                      //
      int height,                                   //
      int width,                                    //
      char populated_character,                     //
      char background_character)                    //
      : height(height), width(width + 1),           // + 1 for the newline char
        _populated_character(populated_character),  //
        _background_character(background_character) //
  {                                                 //

    int canvas_length = height * width;
    _canvas = (char *)malloc(canvas_length * sizeof(char));

    // fill the entire canvas with background_characters and end each line
    // with a newline char
    int true_index = 0;
    for (int height_index = 0; height_index < height; ++height_index) {
      for (int width_index = 0; width_index < width-1; ++width_index) {
        _canvas[true_index] = background_character;
        true_index++;
      }
      _canvas[true_index] = '\n';
      true_index++;
    }
  }
};

class game {
  // put instances in here so rendering works as a state machine
  // rather than constantly re-rendering
};

void asciiCanvas::toggleCoord(int x_coord, int y_coord) {
  int true_index = abs(x_coord + (y_coord * width));
  if (_canvas[true_index] == _populated_character) {
    _canvas[true_index] = _background_character;
  } else {
    _canvas[true_index] = _populated_character;
  }
}

enum keybinds {
  LEFT_BIND = KEY_A,
  RIGHT_BIND = KEY_D,
  UP_BIND = KEY_W,
  DOWN_BIND = KEY_S,
  EXIT_BIND = KEY_Y,
};

void actorObj::handle_key(input_event key_event) {
  switch (key_event.code) {
  case RIGHT_BIND:
    prev_x_position = x_position;
    x_position += move_increment;
    break;
  case LEFT_BIND:
    prev_x_position = x_position;
    x_position -= move_increment;
    break;
  case UP_BIND:
    prev_x_position = x_position;
    y_position += move_increment;
    break;
  case DOWN_BIND:
    prev_x_position = x_position;
    y_position -= move_increment;
    break;
  case EXIT_BIND:
    std::exit(EXIT_SUCCESS);
  }
  return;
}

void asciiCanvas::render() {
  system("clear");
  toggleCoord(objects[0]->x_position, objects[0]->y_position);
  toggleCoord(objects[0]->prev_x_position, objects[0]->prev_y_position);
  std::cout << "press y to exit" << std::endl
            << "xpos: " << objects[0]->x_position << std::endl
            << "ypos: " << objects[0]->y_position << std::endl
            << _canvas << std::endl;
}

int main() {
  // TODO get OS input to decide library to use

  // POSIX (linux, mac) input happens by writing input data to a file
  // your keyboard is literally /dev/input/event2 which is kinda nuts
  const char *dev = "/dev/input/event2";

  // file descriptors are an index in a table of every file
  // linux currently has open and are used as basically
  // a common language between all applications that access files
  int file_dscrptr = open(dev, O_RDONLY);
  if (file_dscrptr == -1) {
    perror("Error opening device");
    return 1;
  }

  struct input_event event;
  int canvas_height = 30;
  int canvas_width = 80;
  char canvas_background = '.';
  char canvas_populated = '#';
  asciiCanvas canvas(canvas_height,      // char height of ascii canvas
                     canvas_width,       // char width of ascii canvas
                     canvas_populated,   // inactive char
                     canvas_background); // active char
  int move_increment = 1;
  actorObj guy(move_increment);
  canvas.objects.push_back(&guy);

  while (0 < read(file_dscrptr, // file descriptor
                  &event,       // give the file data to the input event struct
                  sizeof(struct input_event))) {
    if (event.type == EV_KEY) {
      guy.handle_key(event);
    }
    canvas.render();
  }
  close(file_dscrptr);
  return 0;
}
