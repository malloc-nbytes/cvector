#include "cvector.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
  Cvector cv1 = CVCREATE(int);
  Cvector cv2 = CVCREATE(int);

  for (int i = 0; i < 10; i++) {
    cvector_push(&cv1, &i);
    cvector_push(&cv2, &i);
  }

  for (int i = 9; i >= 0; i--) {
    cvector_push(&cv2, &i);
  }

  printf("%d\n", cvector_shallow_eq(&cv1, &cv2));

  cvector_free(&cv1);
  cvector_free(&cv2);

  return 0;
}
