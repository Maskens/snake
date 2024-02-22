#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#ifndef COMMON_H
#define COMMON_H

typedef enum bool {
  false,
  true
} bool;

typedef struct game {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Game;

#endif // !COMMON_H
