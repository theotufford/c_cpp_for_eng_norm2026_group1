Elijah Crum
Sean Hickey
Faruk hussein
Theo Tufford
CSCI 1113-10
Dan runinguningingenagain

Conway's game of life

    a grid of some size is programmed with logic that allows each cell in the grid to be considered on or off (alive or dead). the player is prompted to turn cells on or off in the grid before the program starts.
    once the program started a set of rules determine sthe state of each cell in the grid. Once the new state of all the cells is determined the rules are applied again. the process repeated until the program is prompted to stop. 

    Rules:

        any cell with fewer than 2 neighbors dies.
        any cell with 2 or 3 neighbors lives.
        any cell with more than 4 neighbors dies.

    the grid:

        the grid is of arbetrary size.
        the edges of the board loops so that the top and bottom
        of the board appear connected as well as the left and right

practicalities:

    creating the grid:

        idk how this works yet.

    displaying the grid:

        i expect the grid will be displayed in a seperate like live doccument for readout hopefully a non theo member can to this to allow for b work.

    storing info on the cells:

        cells will likely be a struct with position and state cells need to know their position and the state of cells around them, as well as themselves.

    implemating game logic:

        this will probably be a fucntion called like cellupdate that can be used as a method for cells as a part of a larger fucntion that preforms these operaitons on the scale of the whole grid. i belive the order in which these operations take place may prove a problem. i dont yet have any specific insights yet.

    updating the grid (game tick):

        nothing is coming to mind here 

    allowing user to create starting conditions:

        it would be cool if you could click on cells to turn them on 

assignments: 

    who should do what 




Faruk - lexical storage of different gol structures
Eli - allow the user to create starting conditions 
Sean - Implementing game logic
theo - general architecture 