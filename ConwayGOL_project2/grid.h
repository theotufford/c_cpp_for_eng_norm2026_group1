#include <cstdlib>
#include <string>
#include <vector>


class coord {
  int x;
  int y;
};

// abstraction layer to work with the binary representation
// of a row 
class Row {
  private:
    // actual width of tracked cells
     const int _width; 
     char * data; // pointer to row data in memory
                     // needs to be a pointer because row width is 
                     // not known at compile time
  public:
  std::vector<int> get_populated_indecies();

  // will return -1 if index is out of range for safety
  int get_status_of(int index);
  int kill_at(int index);
  int populate_at(int index);
  int toggle_at(int index);
  // combines two rows somehow, starting at 0 on the input and 
  // start_index on the output
  int row_union(Row * input, int start_index);
  int replace(Row * input, int start_index);
  int erase(Row * input, int start_index);
  int invert(Row * input, int start_index);

  Row(int width, char * data); // construct row from raw binary data
  ~Row(); // destructor to free the row data (as the row object itself no longer exists)

};

class Grid {
public:
    std::string name; // stored as / inherited from filname
    int width;
    int height;

    coord position;

    std::vector<Row> Grid_data;

    std::vector<coord> get_living;

    void evolve();

    // modify Grid by overlapping an input Grid 
    // and activating any dead cells that are
    // active in the overlapping region
    void grid_union(
        Grid changes,
        coord position
    );
    // overwrite the overlapping region with the input
    void replace_section(
        Grid changes,
        coord position
    );
    // any alive cells in the input region 
    // kill alive cells on this Grid region
    void erase(
        Grid changes,
        coord position
    );
    void invert(
        Grid changes,
        coord position
    );

    int store(); // store as bitmap

    Grid(
        std::string name = "temp",
        int width = 10,
        int height = 10,
        char * binary_rowdata = NULL
    );
};
