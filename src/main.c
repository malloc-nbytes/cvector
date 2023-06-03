#include "cvector.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("%d\n", cvector_index(&cv, CVCONST(2)));
  printf("%d\n", cvector_index(&cv, CVCONST(9)));

  cvector_free(&cv);
  return 0;
}
