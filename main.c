#include "SDL2/SDL_events.h"
#include <SDL2/SDL.h>
#include "src/common.h"
#include "src/init.h"
#include "src/player.h"

struct Pos* spawn_food();

int main(int argc, char *argv[]) {
  bool isRunning = true;
  Game game;

  struct Pos *food = spawn_food();  // Move this to food.c

  if (!Init_SDL(&game)) {
    return 1;
  }

  init_player();

  while (isRunning) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
      }

      if (event.type == SDL_KEYDOWN) {
        set_player_dir(event);
      }
    }

    move_player();

    SDL_RenderClear(game.renderer);

    draw_player(game.renderer);

    // Draw food

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderPresent(game.renderer);
  }

  destroy_player();

  return 0;
}
