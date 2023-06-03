#include "cvector.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_change(&cv, 2, CVCONST(99));

  printf("After:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
