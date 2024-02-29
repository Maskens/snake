#include "SDL2/SDL_events.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_render.h"
#include "src/common.h"
#include "src/init.h"
#include "src/player.h"

struct Pos* spawn_food();
void init_random();
void draw_food(SDL_Renderer *renderer);
int collision(struct BodyPart* player, struct Pos* food);

int main(int argc, char *argv[]) {
  bool isRunning = true;
  Game game;
  struct BodyPart* player;
  struct Pos* food;
  init_random();

  if (!Init_SDL(&game)) {
    return 1;
  }

  food = spawn_food();
  player = init_player();

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
    if(collision(player, food)) {
      SDL_Log("Grow snake!");
    }

    SDL_RenderClear(game.renderer);

    draw_player(game.renderer);
    draw_food(game.renderer);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderPresent(game.renderer);
  }

  destroy_player();

  return 0;
}

int collision(struct BodyPart* player, struct Pos* food) {
  if (((player->x + SIZE) > food->x) &&
      ((player->y + SIZE) > food->y) &&
      (player->x < (food->x + SIZE)) &&
      (player->y < (food->y + SIZE))) {
    return 1;
  } else {
    return 0;
  }
}
