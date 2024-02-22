#include <SDL2/SDL.h>

#ifndef COMMON_H
#define COMMON_H

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef enum bool {
  false,
  true
} bool;

typedef struct game {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Game;

#endif // !COMMON_H
