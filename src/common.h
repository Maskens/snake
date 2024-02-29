#include <SDL2/SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#ifndef COMMON_H
#define COMMON_H

static int SIZE = 16;

typedef enum bool {
  false,
  true
} bool;

struct Pos {
  int x;
  int y;
};

typedef struct game {
  SDL_Window *window;
  SDL_Renderer *renderer;
} Game;

#endif // !COMMON_H
