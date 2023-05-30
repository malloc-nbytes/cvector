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

void mult_by_two(void *elem) { *(int *)elem *= 2; }

void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }

int main(void) {

  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_pushdyn(&cv, &i);
  }

  int s = *(int *)cvector_fold_right(&cv, sum);
  printf("here: %d\n", s);
  printf("here: %d\n", *(int *)cvector_at(&cv, 0));

  cvector_free(&cv);

  return 0;
}
