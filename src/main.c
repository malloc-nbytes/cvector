#include "cvector.h"
#include <stdio.h>

void sum(void *a, void *b) { 
  *(int *)a = *(int *)a + *(int *)b;
}

int main(void) {

  Cvector cv = cvector_create(sizeof(int));

  for (int i = 1; i < 5; i++) {
    cvector_pushdyn(&cv, &i);
  }

  cvector_fold_right(&cv, sum);
  printf("%d\n", *(int *)cvector_at(&cv, 0));

  cvector_free(&cv);

  return 0;
}
