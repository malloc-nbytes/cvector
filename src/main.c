#include "cvector.h"
#include <stdio.h>

struct Point {
  int x;
  int y;
};

struct Point point_create(int x, int y) {
  struct Point p;
  p.x = x;
  p.y = y;
  return p;
}

void point_print(const void *ptr) {
  struct Point *p = (struct Point *)ptr;
  printf("x: %d, y: %d\n", p->x, p->y);
}

int main(void) {

  Cvector cv = cvector_create(sizeof(struct Point));

  struct Point p1 = point_create(1, 2);
  struct Point p2 = point_create(3, 4);

  cvector_push(&cv, &p1);
  cvector_push(&cv, &p2);

  cvector_foreach(&cv, point_print);

  for (int i = 5; i < 10; i++) {
    struct Point tmp = point_create(i, i + 1);
    cvector_pushdyn(&cv, &tmp);
  }

  cvector_foreach(&cv, point_print);

  return 0;
}
