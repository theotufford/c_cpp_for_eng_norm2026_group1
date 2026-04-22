#include "rendering_utils.hpp"
#include <SDL_image.h>
#include <SDL_render.h>
#include <iostream>

Spritesheet::Spritesheet(char const *path, int row, int column) {
  m_spritesheet_image = IMG_Load(path);
  if (!m_spritesheet_image) {
    std::cout << "Failed to load: " << path << " — " << IMG_GetError()
              << std::endl;
    return;
  }
  std::cout << "Loaded image: " << m_spritesheet_image->w << "x"
            << m_spritesheet_image->h << std::endl;
  m_spritesheet_image = IMG_Load(path);

  m_clip.w = m_spritesheet_image->w / column;
  m_clip.h = m_spritesheet_image->h / row;
}
Spritesheet::~Spritesheet() { SDL_FreeSurface(m_spritesheet_image); }

void Spritesheet::select_sprite(int column, int row) {
  m_clip.x = column * m_clip.w;
  m_clip.y = row * m_clip.h;
}

void get_sprite(int column, int row) {}

void Spritesheet::draw_selected_sprite(SDL_Renderer *renderer,
                                       Position_2d *position, double angle) {

  // SDL_BlitSurface(m_spritesheet_image, &m_clip, window_surface, position);
}

LTexture::LTexture() {
  // Initialize
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
}

LTexture::~LTexture() {
  // Deallocate
  free();
}

bool LTexture::loadFromSurface(SDL_Surface *loadedSurface) {
  // Get rid of preexisting texture
  free();

  // The final texture
  SDL_Texture *newTexture = NULL;

  // Load image at specified path
  // Color key image
  SDL_SetColorKey(loadedSurface, SDL_TRUE,
                  SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

  // Create texture from surface pixels
  newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
  if (newTexture == NULL) {
    printf("Unable to create texture! SDL Error: %s\n", SDL_GetError());
  } else {
    // Get image dimensions
    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;
  }

  // Get rid of old loaded surface
  SDL_FreeSurface(loadedSurface);

  // Return success
  mTexture = newTexture;
  return mTexture != NULL;
}

void LTexture::free() {
  // Free texture if it exists
  if (mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
  // Modulate texture rgb
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending) {
  // Set blending function
  SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha) {
  // Modulate texture alpha
  SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle,
                      SDL_Point *center, SDL_RendererFlip flip) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = {x, y, mWidth, mHeight};

  // Set clip rendering dimensions
  if (clip != NULL) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  // Render to screen
  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth() { return mWidth; }

int LTexture::getHeight() { return mHeight; }
