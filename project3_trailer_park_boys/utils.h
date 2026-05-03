#pragma once
#include <string>
#include <vector>
using namespace std;

class Rect;
class intvec2d;
// this is sort of stupid I shouldnt have done any of this 
// but its the way that a real game engine works and it 
// really only ended up in here because I wanted to tool around with it 
// im sure there are infinite associated bugs and this implementation 
// is lazy and not needed but its here anyway and im sick of this project
class vec2d {
public:
  double x = 0;
  double y = 0;
  vec2d operator+(const vec2d &other);
  void rotate_by_rads(double theta);
  vec2d operator-(const vec2d &other);
  vec2d operator*(const double &other);
  bool operator==(const vec2d &other);
  bool is_within_absolute(Rect bound);
  intvec2d floor();
  vec2d(int x, int y);
  vec2d(double x, double y);
  vec2d() = default;
};
// we are on a grid after all
class intvec2d : public vec2d {
public:
  int x = 0;
  int y = 0;
};

// I originally implemented this when I had the naive dream of 
// having sprites that were more than one character and it sort 
// of just is stuck here
// it is also from what I understand, how things are done in a real engine 
// (sort of, SDL)
class Rect {
private:
  const vec2d _initial_center;
  const vec2d &_center;
  double relative_left;
  double relative_right;
  double relative_top;
  double relative_bottom;

public:
  double left();
  double right();
  double top();
  double bottom();
  double width();
  double height();
  const vec2d &get_center();
  bool is_within(Rect sub_rect_candidate);
  bool is_overlapping(Rect sub_rect_candidate);
  Rect(double width, double height, vec2d &center);
  Rect(double bottom, double left, double top, double right);
};
