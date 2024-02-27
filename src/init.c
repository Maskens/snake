#include "common.h"
#include <SDL2/SDL.h>
#include <stdio.h>

bool Init_SDL(Game* game) {
  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Could not init SDL!");
    return false;
  }

  game->window = SDL_CreateWindow("My window",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            0);
  SDL_SetWindowTitle(game->window, "Snake");
  game->renderer = SDL_CreateRenderer(game->window, -1, 0);

  if (game->window == NULL) {
    fprintf(stderr, "Window failed to initialize!: %s", SDL_GetError());
    return false;
  }

  return true;
}
