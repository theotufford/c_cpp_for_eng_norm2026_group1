#pragma once
#include "physics_utils.hpp"
#include <cmath>

// <========== PHYSICS ENGINE UTILITIES ==========>
void Vec_2d::rotate_by_rads(double theta) {
  double oldx = x; 
  x = (x * cos(theta)) - (y * sin(theta));
  y = (oldx * sin(theta)) + (y * cos(theta));
}
void Vec_2d::rotate_by_degs(double theta) {
  theta = theta * 2 * M_PI / 360;
  rotate_by_rads(theta);
}

Vec_2d Vec_2d::operator+(const Vec_2d &other) {
  Vec_2d newVec;
  newVec.x = x + other.x;
  newVec.y = y + other.y;
  return newVec;
}
Vec_2d Vec_2d::operator-(const Vec_2d &other) {
  Vec_2d newVec;
  newVec.x = x - other.x;
  newVec.y = y - other.y;
  return newVec;
}

double Position_2d::get_distance_to(Position_2d &other) {
  Vec_2d distance_vector = *this - other;
  double distance = sqrt(distance_vector.x * distance_vector.x +
                         distance_vector.y * distance_vector.y);
  return distance;
}

