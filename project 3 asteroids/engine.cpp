#include "engine.hpp"
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <cmath>
#include <exception>
#include <stdexcept>

// <========== IMPLEMENTATION / GAME SPECIFIC OBJECTS ==========>

PhysicalObject::PhysicalObject(
    double max_velocity,        // linear velocity limit
    double max_angular_velocity // angular velocity limit
    )
    : //  initializer list, sets these values on creation
      _max_velocity(max_velocity),                //
      _max_angular_velocity(max_angular_velocity) //
{};

// pawns are things with health that can choose to move around
// and take actions, they have health and can die
Pawn::Pawn(
    int initial_health,   // Pawn health
                          // rest of these are just for the parent constructor
    double angular_accel, //
    double forward_accel, //
    double max_forward_velocity, //
    double max_angular_velocity  //
    )
    : _max_health(initial_health), // initialize max health variable
      health(initial_health),      // set health to max
      // you always need to call the parent constructor
      PhysicalObject(           // call physical object parent constructor
          max_forward_velocity, //
          max_angular_velocity  //
      ) {}

Player::Player()
    // call pawn parent constructor with defaults built into the class
    : Pawn(max_health, angular_acceleration, forward_acceleration, velocity_max,
           ang_velocity_max) {}

Asteroid::Asteroid(int size,                // asteroid size group
                   double initial_velocity, // asteroid
                   double initial_angular_velocity)
    : _size(size),    // set asteroid size
      Pawn(           // call parent constructor with class defined constants
          max_health, // class defined max health
          0, 0,       // doesnt accelerate
          initial_velocity,        // velocity limit
          initial_angular_velocity // angular velocity limit
      ) {
  // spawn behavior goes here
  forward_velocity = initial_velocity;
  angular_velocity = initial_angular_velocity;
}

void Player::shoot() {}

// collision responses
void Bullet::collision_response(PhysicalObject &other) {}
void Pawn::collision_response(PhysicalObject &other) {}

void Player::on_death() {}
void Asteroid::on_death() {}

void Asteroid::draw_sprite(SDL_Renderer *renderer) {
  asteroid_spritesheet.select_sprite(_size, 0);
  asteroid_spritesheet.draw_selected_sprite(renderer, &position, angle);
}

void Player::draw_sprite(SDL_Renderer *renderer) {
  if (forward_acceleration != 0) {
    player_spritesheet.select_sprite(1, 0);
  } else {
    player_spritesheet.select_sprite(0, 0);
  }

  player_spritesheet.draw_selected_sprite(renderer, &position, angle);
}

void Bullet::draw_sprite(SDL_Renderer *renderer) {
  bullet_sprite.select_sprite(0, 0);
  bullet_sprite.draw_selected_sprite(renderer, &position, 0);
}

// direct collision damage
void Pawn::take_damage(int damage) {}
