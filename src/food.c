#include <stdlib.h>
#include <time.h>
#include "math.h"
#include "common.h"

time_t t;

void init_random() {
  srand(time(NULL));
}

struct Pos foodPos;

struct Pos* spawn_food() {
  int x = (int) floor((rand() % WINDOW_WIDTH) / 16.0) * 16.0;
  int y = (int) floor((rand() % WINDOW_HEIGHT) / 16.0) * 16.0;

  foodPos.x = x;
  foodPos.y = y;

  return &foodPos;
}

void draw_food(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  SDL_Rect rect;
  rect.x = foodPos.x;
  rect.y = foodPos.y;
  rect.h = SIZE;
  rect.w = SIZE;

  SDL_RenderDrawRect(renderer, &rect);
  SDL_RenderFillRect(renderer, &rect);
}
