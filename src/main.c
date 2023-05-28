#include "cvector.h"
#include <stdio.h>

int compar(const void* a, const void* b) {
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

  Cvector cv = cvector_create_with_cap(100);

  cvector_push(&cv, CVWRAP(2));
  cvector_push(&cv, CVWRAP(4));
  cvector_push(&cv, CVWRAP(3));
  cvector_push(&cv, CVWRAP(1));

  Cvector map = cvector_map(&cv, mult_by_two);

  void *s = cvector_fold_right(&map, sum);

  printf("%d\n", *(int *)s);

  return 0;
}
