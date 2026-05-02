#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

// ANSI Escape Codes
#define RESET   "\033[0m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"

using namespace std;

class mapObject {
    public:
    int x;
    int y;
    char rep = '.';

    /* TODO i need to make a character sub - class, which are things that can move
     i iwant to be able to say move x, y and have that work  */

    /* THIS IS THE CONSTRUCTOR gets the x and y chords of the mapObject for later reference */
    mapObject () {
    /*opening the file*/
        ifstream inFile;
        inFile.open ("display.txt"); 

        /*finding the sprite*/
    string activeLine;

        while (getline (inFile, activeLine )) {
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
            yPos ++;
        }
    inFile.close();
    }  
};

class gameMap {

    /*the storage version of the map*/
    public:
    mapObject posIdentity[20][31] = {};

    /*okay hes how this is going to work you place all your object on start up into the envornment layer
    except for map sprites and interactables. any time a move function is called instead of replacing where your moving from
    with the empty tile '.' it will replace it with wahtever is on the envornment layer.*/

    char enviornmentLayer[20][31] = {
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','.','.','.','.','.','.','%','%','|','.'J'.','.','|','.','.','.','.','.','.','.','.','|','.','.','.','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','%','%','|','#','#','#','|','.','.','.','.','.','.','.','.','|','#','#','#','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','%','%','|','#','#','#','|','.','.','.','.','.','.','.','.','|','#','#','#','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','%','%','|','-','-','-','|','.','.','.','.','.','.','.','.','|','#','#','#','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','%','%','%','%','%','%','.','.','.','.','.','.','.','.','|','-','-','-','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','.','%','%','%','%','%','.','.','.','%','%','%','.','.','%','%','%','%','%','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','%','&','%','.','.','%','%','%','%','%','%','%','.'},
            {'.','.','|','.','.','.','|','%','.','.','.','.','.','.','.','.','.','.','%','%','%','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','|','#','#','#','|','%','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','|','#','#','#','|','%','.','.','.','.','.','.','|','R','.','.','|','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','|','#','#','#','|','%','|','#','#','|','.','.','|','#','#','#','|','.','.','.','.','.','.','.','.','.','.','.','.'},
            {'.','.','|','-','-','-','|','%','|','#','#','|','.','.','|','#','#','#','|','.','.','.','.','.','|','B','.','|','%','%','.'},
            {'.','.','.','.','.','%','%','%','.','.','.','.','.','.','|','-','-','-','|','.','.','.','.','.','|','#','#','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','%','%','%','%','%','.','.','.','.','.','|','#','#','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','%','%','%','%','%','.','.','.','.','.','|','-','-','|','%','%','.'},
            {'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'},
};

    /*the funciton that lets you place map objects*/
    bool placeObject (int row ,int column,char rep) {
        /*the try function is doing edge detection */
        try {
            if (row > 19 || row < 0) {
                throw 0;
            }
            if (column > 30 || column < 0) {
                throw 1;
            }
        }
        catch (int error) {
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
    void updateMap () {
        ofstream outFile;
        outFile.open("display.txt");
        for (int r = 0; r < 20; r++) { // rows
            for ( int c = 0; c < 31; c++) { // columns 
                outFile << (posIdentity[r][c]).rep << ' ';
            }
            outFile << endl;
        }
    outFile.close();
    } 
};

class mapSprite : public mapObject {
    public :
    /*all move does is call the place object function but also run collision detection
    
    TODO: ask fucking theo about this fucking & symbol in the params -- idk i kinda understand why it needs to be there now*/
  bool move (gameMap &activeMap, int moveX, int moveY) {
        int newY = y + moveY;
        int newX = x + moveX;
        
        if (newY < 0 || newY > 19 || newX < 0 || newX > 30) {
            cout << "cant move there out of bounds" << endl;
            return true;
        }

        char dest = activeMap.enviornmentLayer[newY][newX];
        if (dest != '%' && dest != '.' && dest != '#') {
            cout << "cant move there its obstructed" << endl;
            return true;
        }
        
        activeMap.posIdentity[newY][newX].rep = rep;
        activeMap.posIdentity[y][x].rep = activeMap.enviornmentLayer[y][x];
        x = newX;
        y = newY;
        activeMap.updateMap();
        return false;
    } 
    /*the thing that will let you talk to the characters and pick up grills
    TODO right now all interact does it check the tiles 3 x 3 around it for I and return it 
    it will need to be able to call sm that brings up the text window or interaction window or sm*/
    char interact (gameMap &activeMap) {
    
        for (int row = 0; row < 3; row++) {
            for (int column = 0; column < 3; column ++ ) {
                if (activeMap.posIdentity[(y - 1) + row][(x - 1) + column].rep == 'I'/*
                when we add more interactable characters we will expand on this check*/) {
                    cout << "you can interact with that" << endl;
                    return activeMap.posIdentity[(y - 1) + row][(x - 1) + column].rep;
                }
            }
        }
        cout << "nothing to interact with" << endl;
        return 0;
    };
    /*mapSprites ahve their own place fucntion because they only operate on the posIdentity layer
    not on the envornment layer so its importain that they ar not initalized on both*/
    void placeSprite(gameMap &activeMap) {
        activeMap.posIdentity[y][x].rep = rep; 
    }

};

class mapInteractable : public mapObject { // it remains to be seen if this one is needed
public :
/*constructor*/
mapInteractable() {
    rep = 'I';
}
/*saves some work of putting it on the display by hand*/
void placeInteractable (gameMap &activemap) {
activemap.enviornmentLayer[y][x] = rep;
activemap.placeObject(y,x,rep);
}

};

class opp : public mapSprite {
    // Class for movement patterns and vision of lahey
    public :
    int counter = 0; // counter for how many moves they have made in one direction, once it hits max they swap direction
    int max = 15; // this is the number of moves they make in one direction before swapping, can be changed later

    // The direction struct it just holds the x and y direction of movement for the opps, its used in patrol move and swap direction
    struct Direction {
        int x;
        int y;
    };

    // The initial direction of lahey
    Direction direction = {1,0};
    int directionX = 1;
    int directionY = 0;

    // Function that swaps the direction from horizontal to vertical and vice versa, its used in patrol move when the lahey hits a wall or reaches its max moves in one direction
    Direction swapDirection(Direction in) {
        Direction out = {0,0};
        if (in.x == 0) out.x = 1;
        if (in.y == 0) out.y = 1;  
        return out; 
    }

    // The patrol move function moves lahey in the current direction and if it hits a wall 
    // it swaps direction and tries to move again if it hits another wall 
    // it swaps direction again and tries to move if it hits a third wall 
    // it just turns around and goes back the way it came, if it reaches its max moves in one direction it swaps direction
    void patrolMove(gameMap &activeMap) {
        bool observed = !move(activeMap, direction.x, direction.y);
        if (observed) {
            direction.x *= -1;
            direction.y *= -1;
            bool trapped = !move(activeMap, direction.x, direction.y);
            if (trapped) {
                direction = swapDirection(direction);
                bool deadEnd = !move(activeMap, direction.x, direction.y);
                if (deadEnd) {
                    direction.x *= -1;
                    direction.y *= -1;
                }
            } 
            counter = 0;
            return;
        }
        counter ++;
        if (counter == max) {
            swapDirection(direction);
            counter = 0;
        }
    }
    
    
    
    char vision[5][5] = {
        {'!','!','!','!','!'},
        {'!','!','!','!','!'},
        {'!','!',rep,'!','!'},
        {'!','!','!','!','!'},
        {'!','!','!','!','!'},
    };
    bool updateVision(gameMap &activeMap) {

        ofstream outFile;
        outFile.open("display.txt");
        for (int r = 0; r < 20; r++) { // rows
            for ( int c = 0; c < 31; c++) { // columns 
                if (activeMap.posIdentity[r][c].rep == '!') {
                    activeMap.posIdentity[r][c].rep = '.';
                };
                outFile << activeMap.posIdentity[r][c].rep << ' ';
            }
            outFile << endl;
        }
    outFile.close(); 

    /*the constructor declares rep as '.' by defalut so this makes sure the value is current*/
        vision[2][2] = rep;
    /*similar to interaction detection but places '!' on the posIdentity layer within vision range*/
        int Rvision = 0;
        for (int r = (y - 2); r < y + 3; r++) {
            int Cvision = 0;
            for ( int c = (x - 2); c < x + 3; c++) { // columns 
                if (r < 0 || r > 19 || c < 0 || c > 30) {
                    Cvision++;
                    continue;
                }
                if (activeMap.enviornmentLayer[r][c] == '.' && activeMap.posIdentity[r][c].rep != '@') {
                    activeMap.placeObject(r,c,vision[Rvision][Cvision]);
                }
                Cvision++;
            }
            Rvision++; 
        }
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 5; column ++ ) {
            int checkY = (y - 2) + row; // The y value of the tile being checked in the posIdentity layer
            int checkX = (x - 2) + column; // The x value of the tile being checked in the posIdentity layer
            if (checkY < 0 || checkY > 19 || checkX < 0 || checkX > 30) continue; // edge detection for the vision range
            if (activeMap.posIdentity[checkY][checkX].rep == '@' && 
                activeMap.enviornmentLayer[checkY][checkX] != '%' &&
                activeMap.enviornmentLayer[checkY][checkX] != '#'
                /*when we add more interactable characters we will expand on this check*/) {
                cout << "you're caught!" << endl;
                return true;
                }
            }
        }
    return false;
    }
};

// Function to highlight any interactables like grill within the 5x5 area around the player as they move in the enviornment layer using ansi codes
void drawHighlightMap(gameMap &activeMap, mapSprite &player) {
    for (int r = 0; r < 20; r++) {
        for (int c = 0; c < 31; c++) {
            char currentChar = activeMap.posIdentity[r][c].rep;
            
            // Check if within 5x5 range of player and is an interactable
            if (r >= player.y - 2 && r <= player.y + 2 && 
                c >= player.x - 2 && c <= player.x + 2 && 
                currentChar == 'I') {
                cout << YELLOW << BOLD << currentChar << RESET << ' ';
            } else {
                cout << currentChar << ' ';
            }
        }
        cout << endl;
    }
}


int main () {
    /*initalization for the game mostly*/

    /* -- GUIDE TOP MAP SETUP -- 
    first place everthing using gameMap.enviornmentLayer[][] = wahtever character is going to go there
    one ALL of your envornment is placed that includes barriers hiding spots etc then you use a loop to set gameMap.enviornmentLayer[][]
    equal to gameMap.posIdentity[][].rep this will make all the stuff you put on envornment layer appear on screen 
    after that you can place your mapSprites like lahey randy and the kid using mapSprite.placeSprite and interactables like the boys using gameMap.place
    call gameMap.updateMap to make it all appear on the display file and you should be golden. 
    to make the envornment layer and the */
    
    //////////////////////MAP SETUP GOES HERE/////////////////////////
    // declare your maps, sprites, and opps.

    gameMap TrailerPark;
    mapSprite player;
    opp lahey;

    lahey.x = 18;
    lahey.y = 12;
    lahey.max = 30;
    lahey.rep = '0';
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
    for (int r = 0; r < 20; r++) { // rows
        for ( int c = 0; c < 31; c++) { // columns 
            if (TrailerPark.posIdentity[r][c].rep != '@' // you need to add any object you dont want over written to this check
                && TrailerPark.posIdentity[r][c].rep != '(' 
                && TrailerPark.posIdentity[r][c].rep != '('
                && TrailerPark.posIdentity[r][c].rep != '0'
                && TrailerPark.posIdentity[r][c].rep != 'I'
                && TrailerPark.posIdentity[r][c].rep != 'J'
                && TrailerPark.posIdentity[r][c].rep != 'R'
                && TrailerPark.posIdentity[r][c].rep != 'B') {
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
    while (esc == false ) {
      /*the player input switch it takes player input for movment interaction and quiting*/
    cin >> keyinput;
    switch (keyinput) {
    
        case 'w':
            moveX = 0;
            moveY = -1;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'W':
            moveX = 0;
            moveY = -1;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'a':
            moveX = -1;
            moveY = 0;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'A':
            moveX = -1;
            moveY = 0;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 's':
            moveX = 0;
            moveY = 1;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'S':
            moveX = 0;
            moveY = 1;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'd':
            moveX = 1;
            moveY = 0;
            player.move(TrailerPark,moveX,moveY);
            cout << "you are now at " << player.x << " " << player.y << endl;
            break;
        case 'D':
            moveX = 1;
            moveY = 0;
            player.move(TrailerPark,moveX,moveY);
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
    lahey.patrolMove(TrailerPark);
    if (lahey.updateVision(TrailerPark)) {
        esc = true;
    }
    TrailerPark.updateMap();
    }
    return 0;
}
