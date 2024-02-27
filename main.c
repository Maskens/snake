#include "SDL2/SDL_events.h"
#include "SDL2/SDL_log.h"
#include "src/common.h"
#include "src/init.h"
#include "src/player.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  bool isRunning = true;
  Game game;

  if (!Init_SDL(&game)) {
    return 1;
  }

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

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderPresent(game.renderer);
  }

  return 0;
}
