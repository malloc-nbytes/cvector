#include "cvector.h"
#include <stdio.h>
#include <assert.h>

void print(const void *ptr) {
  printf("%s\n", (char *)ptr);
}

int main(void) {
  Cvector cv = cvector_create(sizeof(char *));

  char s1[] = "Hello World!";

  cvector_push(&cv, &s1);

  cvector_foreach(&cv, print);

  cvector_free(&cv);
  return 0;
}
