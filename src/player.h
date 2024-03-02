#include <SDL2/SDL.h>
#include "SDL2/SDL_stdinc.h"
#include "common.h"

struct BodyPart {
  int x;
  int y;
  struct BodyPart *next;
};

void init_player();
void grow_player();
SDL_bool player_collision_food(struct Pos* food);
SDL_bool player_collision_self();
void destroy_player();
void set_player_dir(SDL_Event event);
void move_player();
void draw_player(SDL_Renderer *renderer);
