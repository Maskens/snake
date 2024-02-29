#include <SDL2/SDL.h>

struct BodyPart {
  int x;
  int y;
  struct BodyPart *next;
};

struct BodyPart* init_player();
void destroy_player();
void set_player_dir(SDL_Event event);
void move_player();
void draw_player(SDL_Renderer *renderer);
