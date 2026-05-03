#pragma once
#include "utils.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>

void vec2d::rotate_by_rads(double theta) {
  double oldx = x;
  x = (x * cos(theta)) - (y * sin(theta));
  y = (oldx * sin(theta)) + (y * cos(theta));
}

// this is our B level work
// I (theo) gave a pretty long lecture in the library about operator overloading
// I had already implemented pretty much this exact thing elsewhere
// but all the operator code here besides the == was written by faruk and then
// ported into this version while we were working together
vec2d vec2d::operator+(const vec2d &other) {
  vec2d newVec;
  newVec.x = x + other.x;
  newVec.y = y + other.y;
  return newVec;
}
vec2d vec2d::operator-(const vec2d &other) {
  vec2d newVec;
  newVec.x = x - other.x;
  newVec.y = y - other.y;
  return newVec;
}
vec2d vec2d::operator*(const double &scalar) {
  vec2d newVec;
  newVec.x = x * scalar;
  newVec.y = y * scalar;
  return newVec;
}

vec2d::vec2d(int x, int y) : x(x), y(y) {}
vec2d::vec2d(double x, double y) : x(x), y(y) {}

intvec2d vec2d::floor() {
  intvec2d newVec;
  newVec.x = ::floor(x);
  newVec.y = ::floor(y);
  return newVec;
}

bool vec2d::operator==(const vec2d &other) {
  const double epsilion = 0.01;
  bool epsilion_similar =
      abs((x / other.x) - 1) < epsilion && // x is epsilion close
      abs((y / other.y) - 1) < epsilion;   // y is epsilion close
  return epsilion_similar;
}

// the rest of this file is all just geometry
Rect::Rect(double bottom, double left, double top, double right)
    : _initial_center({(bottom + (float)(top - bottom) / 2.),
                       left + (float)(right - left) / 2.}),
      _center(_initial_center) {
  relative_right = right - _center.x;
  relative_left = left - _center.x;
  relative_top = top - _center.y;
  relative_bottom = bottom - _center.y;
  cout << "creating rectangle at {" << _center.x << "," << _center.y << "}"
       << endl
       << "with width: " << height() << endl
       << "height : " << width() << endl;
}

Rect::Rect(double width, double height, vec2d &center)
    : _initial_center(center), _center(center) {
  relative_left = -width / 2.;
  relative_right = width / 2.;
  relative_top = height / 2.;
  relative_bottom = -height / 2.;
};

double Rect::left() { return _center.x + relative_left; }
double Rect::right() { return _center.x + relative_right; }
double Rect::top() { return _center.y + relative_top; }
double Rect::bottom() { return _center.y + relative_bottom; }
double Rect::width() { return abs(relative_left - relative_right); };
double Rect::height() { return abs(relative_top - relative_bottom); };
const vec2d &Rect::get_center() { return _center; };

bool vec2d::is_within_absolute(Rect bound) {
  return x < bound.right() && x > bound.left() && y < bound.top() &&
         y > bound.bottom();
};

bool Rect::is_within(Rect sub_rect_candidate) {
  vec2d topright = {sub_rect_candidate.right(), sub_rect_candidate.top()};
  vec2d bottomleft = {sub_rect_candidate.left(), sub_rect_candidate.bottom()};
  return topright.is_within_absolute(*this) &&
         bottomleft.is_within_absolute(*this);
}

bool Rect::is_overlapping(Rect sub_rect_candidate) {
  vec2d topright = {sub_rect_candidate.right(), sub_rect_candidate.top()};
  vec2d topleft = {sub_rect_candidate.left(), sub_rect_candidate.top()};
  vec2d bottomright = {sub_rect_candidate.right(), sub_rect_candidate.bottom()};
  vec2d bottomleft = {sub_rect_candidate.left(), sub_rect_candidate.bottom()};
  return topright.is_within_absolute(*this) ||
         bottomleft.is_within_absolute(*this) ||
         topleft.is_within_absolute(*this) ||
         bottomright.is_within_absolute(*this);
}
