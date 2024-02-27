#include <SDL2/SDL.h>

void init_player();
void destroy_player();
void set_player_dir(SDL_Event event);
void move_player();
void draw_player(SDL_Renderer *renderer);
