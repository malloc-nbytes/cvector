#include "cvector.h"
#include <stdio.h>

void print(const void *ptr) { printf("%d\n", *(int *)ptr); }

void add(void *a, void *b) {
  int *ax = (int *)a;
  int *bx = (int *)b;
  printf("%d + %d\n", *ax, *bx);
  *ax = *ax + *bx;
}

int main(void) {

  Cvector cv = cvector_create(sizeof(int));

  for (int i = 1; i < 6; i++) {
    cvector_push(&cv, &i);
  }

  cvector_fold_left(&cv, add);

  printf("%d\n", *(int *)cvector_at(&cv, 0));

  cvector_free(&cv);

  return 0;
}
