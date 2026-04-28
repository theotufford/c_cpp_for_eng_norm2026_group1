#pragma once
#include "physics_utils.hpp"
#include "rendering_utils.hpp"
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <vector>

// <========== PHYSICAL GAME OBJECTS ==========>

class PhysicalObject {
private:
  const double _max_velocity;
  const double _max_angular_velocity;

public:
  Vec_2d velocity_vector; // vector of length 1 pointing direction of velocity
  double angle;           // tracks the angle of the object from initialization
  Vec_2d get_forward_normal(); // returns vector of length 1 pointing in the
                               // direction of object
  Position_2d position;        // physical position

  // current velocity value
  double forward_velocity = 0;
  double angular_velocity = 0;

  void point_at(Position_2d &other);

  PhysicalObject(double max_velocity, double max_angular_velocity);

  virtual void draw_sprite(SDL_Renderer *renderer) = 0;
  virtual void collision_response(PhysicalObject &other) = 0;

  void physics_loop(double delta_T);

};

// <========== IMPLEMENTATION / GAME SPECIFIC OBJECTS ==========>

// general enemy abstract class
class Pawn : public PhysicalObject {
private:
  const double _angular_accel = 0;
  const double _forward_accel = 0;
  int forward_accel_state = NOT_ACCELERATING;
  int angular_accel_state = NOT_ACCELERATING;

  enum { DECELERATING = -1, NOT_ACCELERATING = 0, ACCELERATING = 1 };
  enum { COUNTERCLOCKWISE = -1, CLOCKWISE = 1 };
  const int _max_health;
  int health;

public:
  int bounty; // point reward on kill
  // collision response is identical for all pawns, only death behavior changes
  void collision_response(PhysicalObject &other);
  void take_damage(int damage);

  virtual void on_death() = 0;

  void physics_loop(double delta_T);

  Pawn(int initial_health, // Pawn health

       // rest of these are just for the PhysicalObject constructor
       double angular_accel,        //
       double forward_accel,        //
       double max_forward_velocity, //
       double max_angular_velocity  //
  );
};

class Asteroid : public Pawn {
private:
  static const constexpr uint max_health = 1;
  static const constexpr double forward_acceleration = 0;
  static const constexpr double angular_acceleration = 0;
  static const constexpr double velocity_max = 2;
  static const constexpr double ang_velocity_max = 2;

  const int _size = 0;
  // determined by the spritesheet
  static const constexpr uint sprite_width = 82;
  static const constexpr uint sprite_height = 75;
  Spritesheet asteroid_spritesheet =
      Spritesheet("../assets/asteroids/asteroids_spritesheet.png", sprite_width,
                  sprite_height);

public:
  // should split asteroid in two
  void on_death();
  void draw_sprite(SDL_Renderer *renderer);
  Asteroid(int size, double initial_velocity, double initial_angular_velocity);
};

class Player : public Pawn {
private:
  static const constexpr uint max_health = 5;
  static const constexpr double forward_acceleration = 2;
  static const constexpr double angular_acceleration = 2;
  static const constexpr double velocity_max = 2;
  static const constexpr double ang_velocity_max = 2;
  static const constexpr uint spritesheet_rows = 1;
  static const constexpr uint spritesheet_cols = 2;

  Spritesheet player_spritesheet =
      Spritesheet("../assets/spaceship/spaceship_spritesheet.png",
                  spritesheet_rows, spritesheet_cols);
  uint health = max_health;

public:
  void handle_control_input(int forward_input, int rotational_input);
  void draw_sprite(SDL_Renderer *renderer);
  int score;
  void on_death();
  void shoot();
  Player();
};

class Bullet : public PhysicalObject {
private:
  const PhysicalObject *_owner;
  static const constexpr uint spritesheet_rows = 1;
  static const constexpr uint spritesheet_cols = 1;
  Spritesheet bullet_sprite = Spritesheet("./assets/bullet/bullet_sprite",
                                          spritesheet_rows, spritesheet_cols);

public:
  void collision_response(PhysicalObject &other);
  void draw_sprite(SDL_Renderer *renderer);
};
