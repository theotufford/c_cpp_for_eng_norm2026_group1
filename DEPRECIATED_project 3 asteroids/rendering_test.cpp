#include "engine.hpp"
#include "rendering_utils.hpp"
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <iostream>

/*
  identical to https://thenumb.at/cpp-course/sdl2/01/01.html
  other than replacing cin.get() with cin.get()
  to not depend on windows system calls
*/
using namespace std;

bool init();
void kill();
bool load();

// Pointers to our window and surfaces

int main(int argc, char **args) {

  if (!init())
    return 1;

  Player player;

  player.position.x += 200;
  player.draw_sprite(gWindow_surface);
  SDL_UpdateWindowSurface(window);
  cin.get();
  player.position.x += 100;
  player.draw_sprite(gWindow_surface);
  SDL_UpdateWindowSurface(window);
  cin.get();

  kill();
  return 0;
}

bool init() {
  // See last example for comments
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    cout << "Error initializing SDL: " << SDL_GetError() << endl;
    cin.get();
    return false;
  }

  gWindow =
      SDL_CreateWindow("Example", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (!gWindow) {
    cout << "Error creating window: " << SDL_GetError() << endl;
    cin.get();
    return false;
  }

  // https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
  auto gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL) {
    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
  } else {
    // Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
      printf("SDL_image could not initialize! SDL_image Error: %s\n",
             IMG_GetError());
    }
  }

  gWindow_surface = SDL_GetWindowSurface(window);
  if (!gWindow_surface) {
    cout << "Error getting surface: " << SDL_GetError() << endl;
    cin.get();
    return false;
  }
  return true;
}

void kill() {
  // Quit
  SDL_DestroyWindow(window);
  SDL_Quit();
}
