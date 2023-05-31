#include "cvector.h"
#include <stdio.h>

void elem_double(void *ptr) {
  int *elem = (int *)ptr;
  *elem *= 2;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  Cvector mapped = cvector_map(&cv, elem_double);

  CVPRINT(mapped, int, "%d");

  cvector_free(&mapped);
  return 0;
}
