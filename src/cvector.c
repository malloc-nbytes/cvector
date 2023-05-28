#include "cvector.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *s_malloc(size_t nbytes) {
  void *p = malloc(nbytes);
  if (!p) {
    fprintf(stderr, "ERROR: failed to allocate %zu bytes. Reason: %s\n", nbytes,
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  return p;
}

void *s_realloc(void *ptr, size_t nbytes) {
  void *p = realloc(ptr, nbytes);
  if (!p) {
    fprintf(stderr, "ERROR: failed to reallocate %zu bytes. Reason: %s\n",
            nbytes, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return p;
}

Cvector cvector_create_with_cap(size_t cap) {
  Cvector cv;
  cv.len = 0;
  cv.cap = cap;
  cv.data = (void **)s_malloc(sizeof(void *) * cap);
  return cv;
}

Cvector cvector_create() {
  Cvector cv;
  cv.data = NULL;
  cv.len = cv.cap = 0;
  return cv;
}

void cvector_push(Cvector *cv, void *data) {
  if (cv->len >= cv->cap) {
    cv->cap = (cv->cap == 0) ? 1 : cv->cap * 2;
    cv->data = (void **)s_realloc(cv->data, sizeof(void *) * cv->cap);
  }
  cv->data[cv->len++] = data;
}

void *cvector_peek(Cvector *cv) {
  return cv->data[cv->len - 1];
}

void *cvector_fold_right(Cvector *cv, void (*func)(void *, void *)) {
  assert(cv->len >= 2);
  void *a = NULL, *b = NULL;
  for (size_t i = 0; i < cv->len - 1; i++) {
    if (!i) {
      a = cv->data[i];
    }
    b = cv->data[i + 1];
    func(a, b);
  }
  cvector_free(cv);
  return a;
}

void cvector_rev(Cvector *cv) {
  int start = 0, end = cv->len - 1;
  while (start < end) {
    void *tmp = cv->data[start];
    cv->data[start] = cv->data[end];
    cv->data[end] = tmp;
    start += 1;
    end -= 1;
  }
}

void cvector_qsort(Cvector *cv, int(*compar)(const void *, const void *)) {
  qsort(cv->data, cv->len, sizeof(void*), compar);
}

Cvector cvector_map(Cvector *cv, void (*map_func)(void *)) {
  Cvector mapped = cvector_create();
  for (size_t i = 0; i < cv->len; i++) {
    void *elem = cv->data[i];
    map_func(elem);
    cvector_push(&mapped, cv->data[i]);
  }
  cvector_free(cv);
  return mapped;
}

void *cvector_pop(Cvector *cv) {
  if (!cv->len) {
    return NULL;
  }
  void *save = cvector_peek(cv);
  cv->len -= 1;
  return save;
}

void *cvector_at(Cvector *cv, size_t idx) {
  if (idx >= cv->len) {
    return NULL;
  }
  return cv->data[idx];
}

void cvector_clear(Cvector *cv) {
  cv->len = 0;
}

size_t cvector_len(Cvector *cv) {
  return cv->len;
}

size_t cvector_cap(Cvector *cv) {
  return cv->cap;
}

int cvector_empty(Cvector *cv) {
  return !cv->len;
}

void cvector_free(Cvector *cv) {
  cv->len = cv->cap = 0;
  free(cv->data);
}
