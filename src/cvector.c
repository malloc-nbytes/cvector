#include "cvector.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *_cvector_malloc(size_t nbytes) {
  void *p = malloc(nbytes);
  if (!p) {
    fprintf(stderr, "ERROR: failed to allocate %zu bytes. Reason: %s\n", nbytes,
            strerror(errno));
    exit(EXIT_FAILURE);
  }
  return p;
}

void *_cvector_realloc(void *ptr, size_t nbytes) {
  void *p = realloc(ptr, nbytes);
  if (!p) {
    fprintf(stderr, "ERROR: failed to reallocate %zu bytes. Reason: %s\n",
            nbytes, strerror(errno));
    exit(EXIT_FAILURE);
  }
  return p;
}

Cvector cvector_create(size_t elem_size) {
  Cvector cv;
  cv.data = NULL;
  cv.len = 0;
  cv.cap = 0;
  cv.elem_size = elem_size;
  return cv;
}

void cvector_push(Cvector *cv, void *data) {
  if (cv->len >= cv->cap) {
    cv->cap = !cv->cap ? 1 : cv->cap * 2;
    cv->data = _cvector_realloc(cv->data, cv->cap * cv->elem_size);
  }
  memcpy(cv->data + cv->len * cv->elem_size, data, cv->elem_size);
  cv->len++;
}

void *cvector_peek(Cvector *cv) {
  if (cv->len == 0) {
    return NULL;
  }
  return cvector_at(cv, cv->len - 1);
}

void cvector_pop(Cvector *cv) {
  if (cv->len == 0) {
    return;
  }
  cv->len -= 1;
}

void *cvector_at(Cvector *cv, size_t index) {
  return cv->data + index * cv->elem_size;
}

void cvector_foreach(Cvector *cv, void(foreach_func)(const void *)) {
  for (size_t i = 0; i < cv->len; i++) {
    foreach_func(cvector_at(cv, i));
  }
}

void cvector_set_len(Cvector *cv, size_t len) {
  assert(len <= cv->len);
  cv->len = len;
}

void cvector_fold_right(Cvector *cv, void (*func)(void *, void *)) {
  assert(cv->len >= 2);
  for (size_t i = 1; i < cv->len; i++) {
    func(cvector_at(cv, 0), cvector_at(cv, i));
  }
  cvector_set_len(cv, 1);
}

Cvector cvector_map(Cvector *cv, void (*map_func)(void *)) {
  Cvector mapped = cvector_create(cv->elem_size);
  for (size_t i = 0; i < cv->len; i++) {
    map_func(cvector_at(cv, i));
    cvector_push(&mapped, cvector_at(cv, i));
  }
  cvector_free(cv);
  return mapped;
}

void cvector_free(Cvector *cv) {
  free(cv->data);
  cv->data = NULL;
  cv->len = 0;
  cv->cap = 0;
  cv->elem_size = 0;
}

size_t cvector_len(Cvector *cv) {
  return cv->len;
}

int cvector_empty(Cvector *cv) {
  return !cv->len;
}

size_t cvector_cap(Cvector *cv) {
  return cv->cap;
}
