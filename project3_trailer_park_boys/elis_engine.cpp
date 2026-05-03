#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define MAP_WIDTH 20
#define MAP_HEIGHT 31

// this is all depereciated but I left it in for comparison
class mapObject {
public:
  int x;
  int y;
  char rep = '.';

  /* THIS IS THE CONSTRUCTOR gets the x and y chords of the mapObject for later
   * reference */
  mapObject() {
    /*opening the file*/
    ifstream inFile;
    inFile.open("display.txt");

    /*finding the sprite*/
    string activeLine;

    while (getline(inFile, activeLine)) {
      int yPos;
      if (activeLine.find(rep) != -1) {
        y = yPos;
        for (int xPos = 0; xPos <= activeLine.size() - 1; xPos++) {
          char activeChar = activeLine.at(xPos);
          if (rep == activeChar) {
            x = xPos;
          }
        }
        break;
      }
      yPos++;
    }
    inFile.close();
  }
};

class gameMap {

  /*the storage version of the map*/
public:
  mapObject posIdentity[MAP_WIDTH][MAP_HEIGHT] = {};

  /*okay hes how this is going to work you place all your object on start up
  into the envornment layer except for map sprites and interactables. any time a
  move function is called instead of replacing where your moving from with the
  empty tile '.' it will replace it with wahtever is on the envornment layer.*/

  char enviornmentLayer[MAP_WIDTH][MAP_HEIGHT] = {
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '%', '%', '%', '|', '#', '#', '#',
       '#', '|', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '%', '%', '%', '|', '#', '#', '#',
       '#', '|', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '%', '%', '%', '|', '#', '#', '#',
       '#', '|', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '%', '%', '%', '|', '#', '#', '#',
       '#', '|', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '%', '%', '%', '|', '_', '_', '_',
       '_', '|', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
      {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
       '.', '.', '.', '.', '.', '.', '.', '.', '.'},
  };

  /*the funciton that lets you place map objects*/
  bool placeObject(int row, int column, char rep) {
    /*the try function is doing edge detection */
    try {
      if (row > MAP_WIDTH - 1 || row < 0) {
        throw 0;
      }
      if (column > MAP_HEIGHT - 1 || column < 0) {
        throw 1;
      }
    } catch (int error) {
      if (error == 0) {
        cout << "placeObject call failed, Row out of range" << endl;
        return false;
      }
      if (error == 1) {
        cout << "placeObject call failed, column out of range" << endl;
        return false;
      }
    }
    (posIdentity[row][column]).rep = rep;
    return true;
  }
  /*the function that loads the map*/
  void updateMap() {
    ofstream outFile;
    outFile.open("display.txt");
    for (int r = 0; r < MAP_WIDTH; r++) {    // rows
      for (int c = 0; c < MAP_HEIGHT; c++) { // columns
        outFile << (posIdentity[r][c]).rep << ' ';
      }
      outFile << endl;
    }
    outFile.close();
  }
};

class mapSprite : public mapObject {
public:
  /*all move does is call the place object function but also run collision
  detection*/

  void move(gameMap &activeMap, int moveX, int moveY) {
    int newY = y + moveY;
    int newX = x + moveX;

    if (newY < 0 || newY > MAP_WIDTH - 1 || newX < 0 || newX > MAP_HEIGHT - 1) {
      cout << "cant move there out of bounds" << endl;
      return;
    }

    char dest = activeMap.enviornmentLayer[newY][newX];
    if (dest != '%' && dest != '.' && dest != '#') {
      cout << "cant move there its obstructed" << endl;
      return;
    }
    activeMap.posIdentity[newY][newX].rep = rep;
    activeMap.posIdentity[y][x].rep = activeMap.enviornmentLayer[y][x];
    x = newX;
    y = newY;
    activeMap.updateMap();
  }

  // TODO change so that this enters into dialogue
  char interact(gameMap &activeMap) {

    for (int row = 0; row < 3; row++) {
      for (int column = 0; column < 3; column++) {
        if (activeMap.posIdentity[(y - 1) + row][(x - 1) + column].rep ==
        'I'/*
                when we add more interactable characters we will expand on
                this check*/) {
          cout << "you can interact with that" << endl;
          return activeMap.posIdentity[(y - 1) + row][(x - 1) + column].rep;
        }
      }
    }
    cout << "nothing to interact with" << endl;
    return 0;
  };
  /*mapSprites have their own place fucntion because they only operate on the
  posIdentity layer not on the envornment layer so its importain that they ar
  not initalized on both*/
  void placeSprite(gameMap &activeMap) {
    activeMap.posIdentity[y][x].rep = rep;
  }
};

// TODO move back and forth some distance and then if move is impossible,
// reverse on every move, random coin flip such that 1 in [move width] times it
// toggles from going up down to left right and back and forth
void patrol_move(mapSprite &mover) {}

class mapInteractable
    : public mapObject { // it remains to be seen if this one is needed
public:
  /*constructor*/
  mapInteractable() { rep = 'I'; }
  /*saves some work of putting it on the display by hand*/
  void placeInteractable(gameMap &activemap) {
    activemap.enviornmentLayer[y][x] = rep;
    activemap.placeObject(y, x, rep);
  }
};

class opp : public mapSprite {
public:
  /*the matrix that holds an opp's detection squares*/
  char vision[5][5] = {
      {'!', '!', '!', '!', '!'}, {'!', '!', '!', '!', '!'},
      {'!', '!', rep, '!', '!'}, {'!', '!', '!', '!', '!'},
      {'!', '!', '!', '!', '!'},
  };
  bool updateVision(gameMap &activeMap) {

    ofstream outFile;
    outFile.open("display.txt");
    for (int r = 0; r < MAP_WIDTH; r++) {    // rows
      for (int c = 0; c < MAP_HEIGHT; c++) { // columns
        if (activeMap.posIdentity[r][c].rep == '!') {
          activeMap.posIdentity[r][c].rep = '.';
        };
        outFile << activeMap.posIdentity[r][c].rep << ' ';
      }
      outFile << endl;
    }
    outFile.close();

    /*the constructor declares rep as '.' by defalut so this makes sure the
     * value is current*/
    vision[2][2] = rep;
    /*similar to interaction detection but places '!' on the posIdentity layer
     * within vision range*/
    int Rvision = 0;
    for (int r = (y - 2); r < y + 3; r++) {
      int Cvision = 0;
      for (int c = (x - 2); c < x + 3; c++) {
        if (activeMap.enviornmentLayer[r][c] == '.' &&
            activeMap.posIdentity[r][c].rep != '@') {
          activeMap.placeObject(r, c, vision[Rvision][Cvision]);
        }
        Cvision++;
      }
      Rvision++;
    }
    for (int row = 0; row < 5; row++) {
      for (int column = 0; column < 5; column++) {
        if (activeMap.posIdentity[(y - 2) + row][(x - 2) + column].rep == '@' && 
                activeMap.enviornmentLayer[(y - 2) + row][(x - 2) + column] != '%' &&
                activeMap.enviornmentLayer[(y - 2) + row][(x - 2) + column] != '#'
                /*when we add more interactable characters we will expand on this check*/) {
          cout << "you're caught!" << endl;
          return true;
        }
      }
    }
    return false;
  }
};

int main() {
  /*initalization for the game mostly*/

  /* -- GUIDE TOP MAP SETUP --
  first place everthing using gameMap.enviornmentLayer[][] = wahtever character
  is going to go there one ALL of your envornment is placed that includes
  barriers hiding spots etc then you use a loop to set
  gameMap.enviornmentLayer[][] equal to gameMap.posIdentity[][].rep this will
  make all the stuff you put on envornment layer appear on screen after that you
  can place your mapSprites like lahey randy and the kid using
  mapSprite.placeSprite and interactables like the boys using gameMap.place call
  gameMap.updateMap to make it all appear on the display file and you should be
  golden. to make the envornment layer and the */

  //////////////////////MAP SETUP GOES HERE/////////////////////////
  // declare your maps, sprites, and opps.

  gameMap TrailerPark;
  mapSprite player;
  opp lahey;

  lahey.x = 3;
  lahey.y = 3;
  lahey.rep = 'R';
  lahey.placeSprite(TrailerPark);

  player.x = 0;
  player.y = 0;
  player.rep = '@';
  player.placeSprite(TrailerPark);

  // place your interactables

  mapInteractable grill;
  grill.x = 15;
  grill.y = 0;
  grill.placeInteractable(TrailerPark);

  ////////////////////////////////////////////////////////////////////

  /* here be setup,
  this part will set the envornment layer and posidentity layer equal  */
  for (int r = 0; r < MAP_WIDTH; r++) {    // rows
    for (int c = 0; c < MAP_HEIGHT; c++) { // columns
      if (TrailerPark.posIdentity[r][c].rep !=
              '@' // you need to add any object you dont want over written to
                  // this check
          && TrailerPark.posIdentity[r][c].rep != '(' &&
          TrailerPark.posIdentity[r][c].rep != '(' &&
          TrailerPark.posIdentity[r][c].rep != '0' &&
          TrailerPark.posIdentity[r][c].rep != 'I') {
        TrailerPark.posIdentity[r][c].rep = TrailerPark.enviornmentLayer[r][c];
      }
    }
  }
  lahey.updateVision(TrailerPark);
  TrailerPark.updateMap();
  int moveX;
  int moveY;

  bool esc;
  char keyinput;

  cout << "(WASD), q to escape, e to interact" << endl;
  while (esc == false) {
    /*the player input switch it takes player input for movment interaction and
     * quiting*/
    cin >> keyinput;
    switch (keyinput) {

    case 'w':
      moveX = 0;
      moveY = -1;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'W':
      moveX = 0;
      moveY = -1;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'a':
      moveX = -1;
      moveY = 0;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'A':
      moveX = -1;
      moveY = 0;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 's':
      moveX = 0;
      moveY = 1;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'S':
      moveX = 0;
      moveY = 1;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'd':
      moveX = 1;
      moveY = 0;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'D':
      moveX = 1;
      moveY = 0;
      player.move(TrailerPark, moveX, moveY);
      cout << "you are now at " << player.x << " " << player.y << endl;
      break;
    case 'q':
      esc = true;
      break;
    case 'Q':
      esc = true;
    case 'e':
      player.interact(TrailerPark);
      break;
    case 'E':
      player.interact(TrailerPark);
      break;
    }
    lahey.move(TrailerPark, moveX, moveY);
    if (lahey.updateVision(TrailerPark)) {
      esc = true;
    }
    TrailerPark.updateMap();
  }
  return 0;
}
