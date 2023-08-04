#include "vec.h"
#include <stdio.h>
#include <assert.h>

void print(void *elem) {
  printf("%d\n", *(int *)elem);
}

void mapfunc(void *elem) {
  int val = *(int *)elem;
  if (val % 2 == 0) {
    *(int *)elem = 99;
  }
}

int main(void) {
  Vec v = vec_new(sizeof(int));

  vec_push_const(&v, 1);
  vec_push_const(&v, 2);
  vec_push_const(&v, 3);

  Vec mapvec = vec_map(&v, mapfunc);

  vec_iter(&mapvec, print);

  vec_free(&mapvec);

  return 0;
}
