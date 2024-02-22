#include <SDL2/SDL.h>
#include "src/init.h"
#include "src/common.h"

int main(int argc, char *argv[]) {
  bool isRunning = true;
  Game game;

  if(!Init_SDL(&game)) {
    return 1;
  }

  while(isRunning) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        isRunning = false;
      }
    }

    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.h = 100;
    rect.w = 100;
  
    SDL_RenderClear(game.renderer);

    // Draw snake, move this to snake file
    SDL_SetRenderDrawColor(game.renderer, 0, 204, 0, 255);
    SDL_RenderDrawRect(game.renderer, &rect);
    SDL_RenderFillRect(game.renderer, &rect);
    // End of draw snake ***
    
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderPresent(game.renderer);
  }

  return 0;
}

