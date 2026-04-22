#pragma once

#include "physics_utils.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <cstdint>
#include <string>

// adapted from https://dev.to/noah11012/using-sdl2-spritesheets-46h5

// global renderer instance
extern SDL_Renderer *gRenderer;
extern SDL_Window *gWindow;
extern SDL_Surface *gWindow_surface;

void init_sdl();

class Spritesheet {
public:
  Spritesheet(char const *path, int row, int column);
  ~Spritesheet();

  void select_sprite(int x, int y);
  void draw_selected_sprite(SDL_Renderer *renderer, Position_2d *position,
                            double angle);

private:
  SDL_Rect m_clip;
  SDL_Surface *m_spritesheet_image;
};

// Texture wrapper class
// adapted from https://lazyfoo.net/tutorials/SDL/15_rotation_and_flipping
class LTexture {
public:
  LTexture();  // Initializes variables
  ~LTexture(); // Deallocates memory
  bool loadFromSurface(
      SDL_Surface *loadedSurface); // loads texture from given surface
  void free();                     // Deallocates texture
  void setColor(Uint8 red, Uint8 green, Uint8 blue); // Set color modulation
  void setBlendMode(SDL_BlendMode blending);         // Set blending
  void setAlpha(Uint8 alpha);                        // Set alpha modulation
  void render(int x, int y, SDL_Rect *clip = NULL,
              double angle = 0.0, // Renders texture at given point
              SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
  int getWidth(); // Gets image dimensions
  int getHeight();

private:
  SDL_Texture *mTexture; // The actual hardware texture

  int mWidth; // Image dimensions
  int mHeight;
};
