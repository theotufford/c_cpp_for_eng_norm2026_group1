# project 2 - conways game of life
## Outline
  the primary elements are the grid, the game logic, and the renderer.
  The grid contains no logic and merely stores and returns data about itself.
  The game logic takes in a grid input, and returns a grid output that is one
  "evolution" forward. The renderer takes a grid and displays it on the screen.
## interface features
1. evolution features
    - start
    - stop
    - revert to initial state
2. change environment grid properties
    - change dimensions
    - toggle boundedness
3. select regions of the grid 
    - kill region
    - activate region
    - toggle region
    - move contents of region by dragging or manually inputting a new center
4. manually toggle pixels on the grid via clicking / dragging
    - think of it as a brush system wherein the base case
      would just be a 1x1 populated grid, but could be
      any grid selected from the library
5. select and load other grids as brushes or main environment
    - mini player that previews the behavior of the grid under open bounds
    - grid library that contains saved grids
## grid class
The grid class is built from, and exports to a bitmap. 
it stores its internal data as a vector of Rows, another data type. 
It provides an abstraction layer to more simply handle tasks like killing or 
populating cells via the combination of other grids using something akin to a brush tool.

```cpp
class Grid {
public:
    string name; // stored as / inherited from filname
    int width;
    int height;

    vector<Row> grid_data;

    void evolve();

    // modify grid by overlapping an input grid 
    // and activating any dead cells that are
    // active in the overlapping region
    void union(
        grid changes,
        int position[static 2],
    );
    // overwrite the overlapping region with the input
    void replace_section(
        grid changes,
        int position[static 2]
    );
    // any alive cells in the input region 
    // kill alive cells on this grid region
    void erase(
        grid changes,
        int position[static 2]
    );
    void invert(
        grid changes,
        int position[static 2]
    );

    int store(); // store as .bmp

    grid(
        string name = "temp",
        int width,
        int height,
        char * binary_rowdata = NULL
    )
};
```
### row class
Mainly acts as a proxy to create and interact with binary row data. the size of the data at the data pointer is not the same 
as the width, each position occupies only one bit thus the true size (in bytes) is actually ceil(width/8.0) (ceiling is 
a function that rounds up a float to an int).
```cpp
class Row {
  private:
     const int _width; // useful for functions that check range to not have to calculate
     char * data; // pointer to row data in memory
                     // needs to be a pointer because row width is 
                     // not known at compile time
  public:
  vector<int> get_populated_indecies();

  // will return -1 if index is out of range for safety
  int get_status_of(int index);
  int kill_at(int index);
  int populate_at(int index);
  int toggle_at(int index);
  // combines two rows somehow, starting at 0 on the input and 
  // start_index on the output
  int union(Row * input, int start_index);
  int replace(Row * input, int start_index);
  int erase(Row * input, int start_index);
  int invert(Row * input, int start_index);

  Row(int width, char * data); // construct row from raw binary data
  -Row() // destructor to free the row data (as the row object itself no longer exists)

};
```

### structure for grid binaries
-------------------------------------------
1: width int byte 1
2: width int byte 2
3: width int byte 3
4: width int byte 4
5: first 8 positions in the first row

... : next (width - 8) positions in the first row

(width)/ 8:

... : 0

width/8 + (width) mod 8: 0

width / 8 + (width) mod(8) + 1: first 8 positions in row 2

... : rest of rows following the same structure

-------------------------------------------

