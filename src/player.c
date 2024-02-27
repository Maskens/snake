#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>

static int SIZE = 16;
static int MOVE_SPEED_MS = 500;

struct Pos {
  int x;
  int y;
};

enum Dir {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

static enum Dir currentMoveDirection = RIGHT;
static struct Pos position = { 100, 100 };
static int timeToMove = 0;

void move_player() {
  int curTime = SDL_GetTicks();

  if (curTime < timeToMove) {
    return;
  } else {
    timeToMove = curTime + MOVE_SPEED_MS;
  }

  switch (currentMoveDirection) {
    case UP:
      position.y -= SIZE;
    break;
    case DOWN:
      position.y += SIZE;
    break;
    case LEFT:
      position.x -= SIZE;
    break;
    case RIGHT:
      position.x += SIZE;
    break;
  }
}

void set_player_dir(SDL_Event event) {
  switch(event.key.keysym.sym) {
    case 'w':
      currentMoveDirection = UP;
      break;
    case 'a':
      currentMoveDirection = LEFT;
      break;
    case 's':
      currentMoveDirection = DOWN;
      break;
    case 'd':
      currentMoveDirection = RIGHT;
      break;
  }
}

void draw_player(SDL_Renderer *renderer) {
    SDL_Rect rect;
    rect.x = position.x;
    rect.y = position.y;
    rect.h = SIZE;
    rect.w = SIZE;
  
    SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);

}
