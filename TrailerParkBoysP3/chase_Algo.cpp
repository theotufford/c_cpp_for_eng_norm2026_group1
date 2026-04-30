#ifndef CHASE_ALGO
#define CHASE_ALGO

///// I'm writing this as a header file to be copied to the source file, which may or may not be helpful. /////

#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

/**
 * @brief This function tracks the distance between the player, and Randy, who is dangerous.
 * @param x1,x2 represents the x-coordinates of two different points.
 * @param y1, y2 represents the y-coordinates of two different points.
 * @return Returns the distance between the player and Randy.
 */
int track_Distance(int x1, int y1, int x2, int y2) {        // I think this function could come in handy later on.
    int dx = x2 - x1;
    int dy = y2 - y2;
    float distance = sqrt(pow(dx, 2) + pow(dy, 2));
    return distance;
}

bool chase;

/**
 * @brief this function alerts randy when the player has entered his territory, and calls the chase function to start the action.
 *        Obviously, the functions are not streamed corrected, nor do they represent the correct locations, and the parameters need to be adjusted.
 * @param vision represents the 5 x 5 array surrounding randy, which is hostile territory for the player.
 */
void randy_Alert(char vision[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (vision[i][j] == '@' && vision[i][j] != 'rep') {
                chase = true;
                break;
            } 
            else {
                chase = false;
            }
        }
    }
    enemy_moves(chase, player.x, player.y, randy.x, randy.y);       // I'd like to call the enemy_moves function here once the chase bool has been set to true.
}



// Randy's movements will oppose the player's unless the player moves towards him.
/**
 * @brief this function will move Randy towards the player, similar to a binary search
 *        I wasn't sure of how to format this yet to include the coordinates of the player and Randy,
 *        as well as how to move Randy around walls.
 *        So far, Randy only moves in accordance to his orientation to the player, when in chase mode
 *        Chase mode is activated when the player enters randy's territory, or the space around the grill he's circling.
 *        Randy only chases the player for 5 moves, as of right now.
 * @param chase The condition allowing randy to chase the player.
 * @param x,y The coordinates of the player and randy on the grid map.
 */
void enemy_Moves(bool chase, int player.x, int player.y, int randy.x, int randy.y) {        //////// THESE NEED TO BE CHANGED, I wasn't sure of how to implement the parameters
    if (chase == true) {
        int step_count = 0;                                                                 // Step_count will initialize at 0, then loop randy's movement until 5 moves have been made.
        while (step_count < 5) {
            track_distance(player.x, player.y, randy.x, randy.y);    // FIX
            if (randy.y < player.y) {
                randy.y += 1;
                step_count += 1;
                return;
            } 
            else if (randy.y > player.y) {
                randy.y -= 1;
                step_count += 1;
                return;
            } 
            else if (randy.x < player.x) {
                randy.x += 1;
                step_count += 1;
                return;
            }
            else if (randy.x > player.x) {
                randy.x -= 1;
                step_count += 1;
                return;
            }
        }
    }
}







#endif