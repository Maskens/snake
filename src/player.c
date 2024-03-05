#include "player.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_log.h"
#include "SDL2/SDL_pixels.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_timer.h"
#include <SDL2/SDL.h>

static int MOVE_SPEED_MS = 100;

enum Dir { UP, DOWN, LEFT, RIGHT };

static struct Pos START_POS = {160, 160};
static struct SDL_Color playerColor = { 0, 204, 0, 255 };
static enum Dir currentMoveDirection = RIGHT;
static int timeToMove = 0;
static int canChangeDir = 1;
static int shouldGrow = 0;
static int playerBlinkSpeed = 200;
static SDL_TimerID blinkTimer;

struct BodyPart *head;

void init_player() {
  SDL_RemoveTimer(blinkTimer);
  currentMoveDirection = RIGHT;
  blinkTimer = 0;

  playerColor.r = 0;
  playerColor.g = 204;
  playerColor.b = 0;
  playerColor.a = 255;

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
  struct BodyPart *curPtr = head;

  while (curPtr->next != NULL) {
    struct BodyPart *oldPtr = curPtr;
    SDL_Log("Destroy node");
    curPtr = curPtr->next;
    free(oldPtr);
  }

  free(curPtr);
}

static void wrapSnake() {
  struct BodyPart *curPtr = head;
  while (1) {
    if (curPtr->x >= WINDOW_WIDTH) {
      curPtr->x = 0;
    }

    if (curPtr->x < 0) {
      curPtr->x = WINDOW_WIDTH - SIZE;
    }

    if (curPtr->y >= WINDOW_HEIGHT) {
      curPtr->y = 0;
    }

    if (curPtr->y < 0) {
      curPtr->y = WINDOW_HEIGHT - SIZE;
    }

    curPtr = curPtr->next;

    if (curPtr == NULL) {
      break;
    }
  }
}

static void moveBodyParts(struct Pos deltaPos) {
  int prevX = head->x;
  int prevY = head->y;

  // Move head with delta pos
  head->x += deltaPos.x;
  head->y += deltaPos.y;

  // Move rest of body
  struct BodyPart *nextPtr = head->next;

  while (1) {
    int tempX = nextPtr->x;
    int tempY = nextPtr->y;

    nextPtr->x = prevX;
    nextPtr->y = prevY;

    prevX = tempX;
    prevY = tempY;

    nextPtr = nextPtr->next;

    if (nextPtr == NULL) {
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

  struct Pos deltaPos = {0, 0};

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

void grow_player() { shouldGrow = 1; }

void set_player_dir(SDL_Event event) {

  if (!canChangeDir) { // Fix so that player cannot move against itself, not the
                       // best fix
    return;
  }

  switch (event.key.keysym.sym) {
  case 'w':
    if (currentMoveDirection != DOWN) {
      currentMoveDirection = UP;
    }
    break;
  case 'a':
    if (currentMoveDirection != RIGHT) {
      currentMoveDirection = LEFT;
    }
    break;
  case 's':
    if (currentMoveDirection != UP) {
      currentMoveDirection = DOWN;
    }
    break;
  case 'd':
    if (currentMoveDirection != LEFT) {
      currentMoveDirection = RIGHT;
    }
    break;
  }

  canChangeDir = 0;
}

void draw_player(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 
                         playerColor.r, 
                         playerColor.g, 
                         playerColor.b, 
                         playerColor.a);

  struct BodyPart *curPtr = head;

  while (1) {
    SDL_Rect rect;
    rect.x = curPtr->x;
    rect.y = curPtr->y;
    rect.h = SIZE;
    rect.w = SIZE;

    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    curPtr = curPtr->next;

    if (curPtr == NULL) {
      break; // We are done
    }
  }
}

SDL_bool player_collision_self() {
  struct SDL_Rect headRect = {head->x, head->y, SIZE, SIZE};
  struct BodyPart *bodyPtr = head->next;

  while(1) {
    SDL_Rect nextBodyPart = { bodyPtr->x, bodyPtr->y, SIZE, SIZE };

    if (SDL_HasIntersection(&headRect, &nextBodyPart)) {
      return SDL_TRUE;
    }

    if(bodyPtr->next == NULL) {
      break;
    }

    bodyPtr = bodyPtr->next;
  } 
  return SDL_FALSE;
}

Uint32 blink(Uint32 interval, void* empty) {
  static int toggle = 0;

  if(toggle) {
    playerColor.r = 0;
    playerColor.g = 0;
    playerColor.b = 0;
    playerColor.a = 255;
  } else {
    playerColor.r = 0;
    playerColor.g = 204;
    playerColor.b = 0;
    playerColor.a = 255;
  }

  toggle = !toggle;

  return interval;
}

void blink_player() {

  if(blinkTimer == 0) {
    blinkTimer = SDL_AddTimer(playerBlinkSpeed, &blink, NULL);
    SDL_Log("%d", blinkTimer);
  }
}

SDL_bool player_collision_food(struct Pos *food) {
  SDL_Rect foodRect = {food->x, food->y, SIZE, SIZE};
  SDL_Rect headRect = {head->x, head->y, SIZE, SIZE};

  return SDL_HasIntersection(&foodRect, &headRect);
}
