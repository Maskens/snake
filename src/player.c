#include "SDL2/SDL_events.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>
#include "player.h"

static int MOVE_SPEED_MS = 100;

enum Dir {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

static struct Pos START_POS = { 160, 160 };
static enum Dir currentMoveDirection = RIGHT;
static int timeToMove = 0;
static int canChangeDir = 1;
static int shouldGrow = 0;

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

static void wrapSnake() {
  struct BodyPart *curPtr = head;
  while(1) {
    if(curPtr->x >= WINDOW_WIDTH) {
      curPtr->x = 0;
    }

    if(curPtr->x < 0) {
      curPtr->x = WINDOW_WIDTH - SIZE;
    }

    if(curPtr->y >= WINDOW_HEIGHT) {
      curPtr->y = 0;
    }

    if(curPtr->y < 0) {
      curPtr->y = WINDOW_HEIGHT - SIZE;
    }

    curPtr = curPtr->next;

    if(curPtr == NULL) {
      break;
    }
  }
}

static void moveBodyParts(struct Pos deltaPos) {
  int prevX = head->x;
  int prevY = head->y;

  //Move head with delta pos
  head->x += deltaPos.x;
  head->y += deltaPos.y;

  //Move rest of body
  struct BodyPart *nextPtr = head->next;

  while(1) {
    int tempX = nextPtr->x;
    int tempY = nextPtr->y;

    nextPtr->x = prevX;
    nextPtr->y = prevY;

    prevX = tempX;
    prevY = tempY;
  
    nextPtr = nextPtr->next;

    if(nextPtr == NULL) {
      break; // We are done
    }
  }

  wrapSnake();
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

  if (shouldGrow) {
    struct BodyPart *newHead = malloc(sizeof(struct BodyPart));

    newHead->x = head->x + deltaPos.x;
    newHead->y = head->y + deltaPos.y;

    newHead->next = head;

    head = newHead;

    shouldGrow = 0;
  }

  moveBodyParts(deltaPos);

  canChangeDir = 1;
}

void grow_player() {
  shouldGrow = 1;
}

void set_player_dir(SDL_Event event) {

  if(!canChangeDir) { //Fix so that player cannot move against itself, not the best fix
    return;
  }

  switch(event.key.keysym.sym) {
    case 'w':
      if(currentMoveDirection != DOWN) {
        currentMoveDirection = UP;
      }
      break;
    case 'a':
      if(currentMoveDirection != RIGHT) {
        currentMoveDirection = LEFT;
      }
      break;
    case 's':
      if(currentMoveDirection != UP) {
        currentMoveDirection = DOWN;
      }
      break;
    case 'd':
      if(currentMoveDirection != LEFT) {
        currentMoveDirection = RIGHT;
      }
      break;
  }

  canChangeDir = 0;
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

int collision_food(struct Pos* food) {
  if (((head->x + SIZE) > food->x) &&
      ((head->y + SIZE) > food->y) &&
      (head->x < (food->x + SIZE)) &&
      (head->y < (food->y + SIZE))) {
    return 1;
  } else {
    return 0;
  }
}

