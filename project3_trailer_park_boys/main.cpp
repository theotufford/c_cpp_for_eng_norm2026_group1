#include "engine.cpp"
#include "utils.cpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <sstream>
#include <string>

int main() {
  // load map
  gameMap map = generate_map_from_file("./display.txt");
  // give us a reference to the player
  Player &player = *map.player;
  char input;
  while (true) {
    // display info 
    cout << "\033[H\033[2J"; // clears screen
    cout << "welcome to the trailer park\n youre the @ - go find Ricky (R) or Julian (J)" << endl;
    cout << "   ";
    for (int i = 0; i < map.width + 10; i++)
      cout << "--";
    cout << endl;
    cout << map.render_to_string() << endl;
    cout << "   ";
    for (int i = 0; i < map.width + 10; i++)
      cout << "--";
    cout << endl;
    cout << "wasd enter input to move, unfortunately key detection isnt portable" << endl;

    // iterate through live objects looking for NPCs
    for (gameObject *candidate : map.live_objects) {
    // most of the behavior around polymorphism youll see throughout this code 
    // is the orphaned child of my own optimism about making this more fun but 
    // its saturday night and I've implemented everyones individual work and 
    // anything more is just me torturing myself
      auto current_npc = dynamic_cast<NPC *>(candidate);
      if (current_npc) {
        current_npc->patrol_move();
      }
    }
    cin.clear();
    player.detection_check();
    cin >> input;
    switch (tolower(input)) {
    case 'w':
      player.direction = {0, -1};
      break;
    case 'a':
      player.direction = {-1, 0};
      break;
    case 's':
      player.direction = {0, 1};
      break;
    case 'd':
      player.direction = {1, 0};
      break;
    }
    player.forward(1);
  }
}
