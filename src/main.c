#include "cvector.h"
#include <stdio.h>
#include <assert.h>

void print(void *elem) {
  printf("%d\n", *(int *)elem);
}

int main(void) {
  Vec v = vec_new(sizeof(int));

  vec_push_const(&v, 1);

  *(int *)vec_at(&v, 0) = 23;

  vec_iter(&v, print);

  vec_free(&v);

  return 0;
}
