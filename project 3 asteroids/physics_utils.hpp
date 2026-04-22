#pragma once
// <========== PHYSICS ENGINE UTILITIES ==========>
class Vec_2d {
public:
  class Position_2d;

  double x = 0;
  double y = 0;

  void norm();
  double get_angle();
  void rotate_by_rads(double theta);
  void rotate_by_degs(double theta);

  // add and subtract two vectors
  Vec_2d operator+(const Vec_2d &other);
  Vec_2d operator-(const Vec_2d &other);

  // scale vectors by const
  Vec_2d operator*(const double &other);
};

// prevents non-physical transformation of position
class Position_2d : public Vec_2d {
public:
  void rotate(double theta) {
    throw "rotating a position vector!! (dont do that)";
  }
  void norm() { throw "trying to normalize literal position!"; }
  double get_distance_to(Position_2d &other);
};

