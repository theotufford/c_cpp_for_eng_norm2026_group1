#include "grid.h"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

void zeroLastBits(char *data, size_t size, int numOfBits) {
  if (numOfBits) {

  }
  char * lastbyte = &data[(size / sizeof(char)) - 1];
  *lastbyte = *data & (1 << numOfBits);
}

Row::Row(int width, char *input_data) : _width(width) {
  size_t true_size = std::ceil(width / 8.f);
  memcpy(data, input_data, true_size);
}
