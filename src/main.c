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

int main(void) {

  Cvector cv = cvector_create_with_cap(100);

  cvector_push(&cv, CV_COMPOUND_LITERAL(2));
  cvector_push(&cv, CV_COMPOUND_LITERAL(4));
  cvector_push(&cv, CV_COMPOUND_LITERAL(3));
  cvector_push(&cv, CV_COMPOUND_LITERAL(1));

  CVECTOR_PRINT(&cv, int, "%d");

  return 0;
}
