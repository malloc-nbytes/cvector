#include "cvector.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CVPANIC(msg, stream, ...)                                             \
  do {                                                                        \
    fprintf(stream, "PANIC [%s:%s:%d]: " msg "\n", __FILE__, __func__,        \
            __LINE__, ##__VA_ARGS__);                                         \
    exit(EXIT_FAILURE);                                                       \
  } while (0)

void *_cvector_malloc(size_t nbytes) {
  void *p = malloc(nbytes);
  if (!p) {
    CVPANIC("failed to allocate %zu bytes. Reason: %s", stderr, nbytes, strerror(errno));
  }
  return p;
}

void *_cvector_realloc(void *ptr, size_t nbytes) {
  void *p = realloc(ptr, nbytes);
  if (!p) {
    CVPANIC("failed to reallocate %zu bytes. Reason: %s", stderr, nbytes, strerror(errno));
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

Cvector cvector_with_capacity(size_t cap, size_t elem_size) {
  Cvector cv;
  cv.data = _cvector_malloc(cap * elem_size);
  cv.len = 0;
  cv.cap = cap;
  cv.elem_size = elem_size;
  return cv;
}

void cvector_remove(Cvector *cv, size_t index) {
  if (index >= cv->len) {
    CVPANIC("index %zu out of bounds", stderr, index);
  }
  memmove(cvector_at(cv, index), cvector_at(cv, index + 1),
          (cv->len - index - 1) * cv->elem_size);
  cv->len--;
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
  if (cv->len != 0) {
    cv->len -= 1;
  }
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
  if (len > cv->len) {
    CVPANIC("new length %zu is greater than current length %zu", stderr, len, cv->len);
  }
  cv->len = len;
}

void cvector_fold_right(Cvector *cv, void (*func)(void *, void *)) {
  if (cv->len < 2) {
    CVPANIC("cannot fold a vector with length less than 2", stderr);
  }
  for (size_t i = 1; i < cv->len; i++) {
    func(cvector_at(cv, 0), cvector_at(cv, i));
  }
  cvector_set_len(cv, 1);
}

void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *)) {
  qsort(cv->data, cv->len, cv->elem_size, compar);
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

void cvector_rev(Cvector *cv) {
  int start = 0, end = cv->len - 1;
  char tmp[cv->elem_size];
  while (start < end) {
    memcpy(tmp, cv->data + start * cv->elem_size, cv->elem_size);
    memcpy(cv->data + start * cv->elem_size, cv->data + end * cv->elem_size,
           cv->elem_size);
    memcpy(cv->data + end * cv->elem_size, tmp, cv->elem_size);
    start += 1;
    end -= 1;
  }
}

void cvector_free(Cvector *cv) {
  free(cv->data);
  cv->data = NULL;
  cv->len = 0;
  cv->cap = 0;
  cv->elem_size = 0;
}

void cvector_clear(Cvector *cv) {
  cv->len = 0;
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

size_t cvector_elem_size(Cvector *cv) {
  return cv->elem_size;
}