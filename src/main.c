#include "cvector.h"
#include <stdio.h>
#include <assert.h>

int compar(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

int main(void) {
  Cvector cv = CVCREATE(int);

  for (int i = 5; i >= 0; i--) {
    cvector_push(&cv, &i);
  }

  CVPRINT(cv, int, "%d");
  cvector_rev(&cv);
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);

  return 0;
}
