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
  int *value = (int *)element;
  *value *= 2;
}

void sum(void *a, void *b) {
  int *ax = (int *)a;
  int *bx = (int *)b;
  *ax = *ax + *bx;
}

int main(void) {

  Cvector cv = cvector_create(sizeof(int));

  cvector_push(&cv, CVWRAP(1));
  cvector_push(&cv, CVWRAP(2));
  cvector_push(&cv, CVWRAP(3));

  for (int i = 4; i < 10; i++) {
    cvector_pushvar(&cv, &i, sizeof(int));
  }

  cvector_rev(&cv);

  Cvector mapped = cvector_map(&cv, mult_by_two);

  CVECTOR_PRINT(&mapped, int, "%d");

  // CVECTOR_PRINT(&cv, int, "%d");
  // cvector_free(&cv);
  cvector_free(&mapped);

  return 0;
}
