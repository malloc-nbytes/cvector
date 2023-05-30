#include "cvector.h"
#include <stdio.h>

void find_vowels(void *elem) {
  char *s = (char *)elem;
  for (int i = 0; s[i] != '\0'; i++) {
    char c = s[i];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
      s[i] = 'x';
    }
  }
}

int main(void) {

  Cvector cv = cvector_create(sizeof(char *));

  cvector_push(&cv, CVWRAP("This is a test"));
  cvector_push(&cv, CVWRAP("Hello world!"));
  cvector_push(&cv, CVWRAP("Foo Bar Baz"));

  CVECTOR_PRINT(&cv, char *, "%s");

  Cvector mapped = cvector_map(&cv, find_vowels);

  CVECTOR_PRINT(&mapped, char *, "%s");

  cvector_free(&mapped);

  return 0;
}
