#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>

static int SIZE = 16;
static int MOVE_SPEED_MS = 200;

struct BodyPart {
  int x;
  int y;
  struct BodyPart *next;
};

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

static struct Pos START_POS = { 160, 160 };
static enum Dir currentMoveDirection = RIGHT;
static int timeToMove = 0;

struct BodyPart *head;

void init_player() {
  struct BodyPart *part2 = malloc(sizeof(struct BodyPart));
  part2->x = START_POS.x - SIZE - SIZE;
  part2->y = START_POS.y;
  part2->next = NULL;

  struct BodyPart *part1 = malloc(sizeof(struct BodyPart));
  part1->x = START_POS.x - SIZE;
  part1->y = START_POS.y;
  part1->next = part2;

  head = malloc(sizeof(struct BodyPart));
  head->x = START_POS.x;
  head->y = START_POS.y;
  head->next = part1;
}

void destroy_player() {
  // Free memory
  struct BodyPart* curPtr = head;

  while(curPtr->next != NULL) {
    struct BodyPart* oldPtr = curPtr;
    SDL_Log("Destroy node");
    curPtr = curPtr->next;
    free(oldPtr);
  }

    SDL_Log("Destroy node");
  free(curPtr);
}

static void updateBodyPartsPos(struct Pos deltaPos) {
  struct BodyPart *curPtr = head;

  while(1) {
    curPtr->x += deltaPos.x;
    curPtr->y += deltaPos.y;
  
    curPtr = curPtr->next;

    if(curPtr == NULL) {
      break; // We are done
    }
  }
}

void move_player() {
  int curTime = SDL_GetTicks();

  if (curTime < timeToMove) {
    return;
  } else {
    timeToMove = curTime + MOVE_SPEED_MS;
  }

  struct Pos deltaPos = { 0, 0 };

  switch (currentMoveDirection) {
    case UP:
      deltaPos.y -= SIZE;
    break;
    case DOWN:
      deltaPos.y += SIZE;
    break;
    case LEFT:
      deltaPos.x -= SIZE;
    break;
    case RIGHT:
      deltaPos.x += SIZE;
    break;
  }

  updateBodyPartsPos(deltaPos);
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
  SDL_SetRenderDrawColor(renderer, 0, 204, 0, 255);

  struct BodyPart *curPtr = head;

  while(1) {
    SDL_Rect rect;
    rect.x = curPtr->x;
    rect.y = curPtr->y;
    rect.h = SIZE;
    rect.w = SIZE;
  
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    curPtr = curPtr->next;

    if(curPtr == NULL) {
      break; // We are done
    }
  }
}
