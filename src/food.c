#include <stdlib.h>
#include <time.h>
#include "common.h"

time_t t;

void init_random() {
  srand(time(&t));
}

struct Pos* spawn_food() {
  struct Pos* food = malloc(sizeof(struct Pos));
  food->x = rand() % WINDOW_WIDTH;
  food->y = rand() % WINDOW_HEIGHT;

  return food;
}
