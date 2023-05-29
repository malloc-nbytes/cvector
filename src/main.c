#include "cvector.h"
#include <stdio.h>

int compar(const void *a, const void *b) {
  int n1 = CVQSORT_COMPARFUNC_CAST(a, int);
  int n2 = CVQSORT_COMPARFUNC_CAST(b, int);
  if (n1 < n2) {
    return -1;
  } else if (n1 > n2) {
    return 1;
  } else {
    return 0;
  }
}

void mult_by_two(void *element) {
  float *value = (float *)element;
  *value *= 2;
}

void sum(void *a, void *b) {
  int *ax = (int *)a;
  int *bx = (int *)b;
  *ax = *ax + *bx;
}

int main(void) {

  Cvector cv = cvector_create(sizeof(int));

  cvector_push(&cv, CVWRAP(10));
  cvector_push(&cv, CVWRAP(9));
  cvector_push(&cv, CVWRAP(8));

  for (int i = 7; i >= 0; i--) {
    cvector_pushdyn(&cv, &i);
  }

  CVECTOR_PRINT(&cv, int, "%d");

  cvector_free(&cv);

  return 0;
}
