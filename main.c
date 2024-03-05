#include "SDL2/SDL_events.h"
#include <SDL2/SDL.h>
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_timer.h"
#include "src/common.h"
#include "src/init.h"
#include "src/player.h"

struct Pos* spawn_food();
void init_random();
void draw_food(SDL_Renderer *renderer);

typedef enum GameState {
  PLAYING,
  DIED,
  RESTART
} GameState;

static GameState gameState = PLAYING;

Uint32 callback(Uint32 interval, void* foo) {
  SDL_Log("Restart!");
  gameState = RESTART;
  return 0;
}

int main(int argc, char *argv[]) {
  SDL_bool isRunning = SDL_TRUE;
  Game game;
  struct BodyPart* player;
  struct Pos* food;
  init_random();
  SDL_TimerID timerId;

  if (!Init_SDL(&game)) {
    return 1;
  }

  food = spawn_food();
  init_player();

  while (isRunning) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = SDL_FALSE;
      }

      if (event.type == SDL_KEYDOWN) {
        set_player_dir(event);
      }
    }

    switch(gameState) {
      case PLAYING:
        move_player();
        if(player_collision_food(food)) {
          grow_player();
          food = spawn_food();
        }

        if(player_collision_self()) {
          timerId = SDL_AddTimer(
            DEATH_ANIM_LENGTH_MS, 
            &callback, 
            NULL
          );
          gameState = DIED;
        }
        break;
      case DIED:
        blink_player();
      break;
      case RESTART:
        destroy_player();
        food = spawn_food();
        init_player();
        gameState = PLAYING;
        break;
    } 

    SDL_RenderClear(game.renderer);

    draw_player(game.renderer);
    draw_food(game.renderer);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderPresent(game.renderer);
  }

  SDL_RemoveTimer(timerId);
  destroy_player();

  return 0;
}
