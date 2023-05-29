#include "cvector.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PANIC(msg, stream)                                                     \
  fprintf(stream, "PANIC: %s in [%s:%d] %s()\n", msg, __FILE__, __LINE__,      \
          __FUNCTION__);                                                       \
  exit(1);

#define TODO(msg, stream)                                                      \
  fprintf(stream, "TODO: %s in [%s:%d] %s()\n", msg, __FILE__, __LINE__,       \
          __FUNCTION__);                                                       \
  exit(1);

#define UNIMPLEMENTED(msg, stream)                                             \
  fprintf(stream, "UNIMPLEMENTED: %s in [%s:%d] %s()\n", msg, __FILE__,        \
          __LINE__, __FUNCTION__);                                             \
  exit(1);

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

Cvector cvector_with_capacity(size_t cap, size_t elem_size) {
  Cvector cv;

  cv.elem_size = elem_size;
  cv.len = 0;
  cv.cap = cap;

  cv.data = (void **)s_malloc(sizeof(void *) * cap);
  cv.allocd = (int *)s_malloc(sizeof(int) * cap);

  cv.allocd_cap = cap;

  memset(cv.allocd, 0, sizeof(cv.allocd[0]) * cap);

  return cv;
}

Cvector cvector_create(size_t elem_size) {
  Cvector cv;

  cv.data = NULL;
  cv.elem_size = elem_size;
  cv.len = cv.cap = 0;

  cv.allocd = NULL;
  cv.allocd_cap = 0;

  return cv;
}

void cvector_push(Cvector *cv, void *data) {
  if (cv->len >= cv->cap) {
    cv->cap = (cv->cap == 0) ? 1 : cv->cap * 2;
    cv->allocd_cap = cv->cap;

    cv->data = (void **)s_realloc(cv->data, sizeof(void *) * cv->cap);
    cv->allocd = (int *)s_realloc(cv->allocd, sizeof(int) * cv->allocd_cap);

    size_t new_elements = (cv->allocd_cap - cv->cap) * sizeof(int);
    memset(cv->allocd + cv->cap, 0, new_elements);
  }
  cv->data[cv->len++] = data;
  cv->allocd[cv->len - 1] = 0;
}

void cvector_pushdyn(Cvector *cv, void *data) {
  void *copy = s_malloc(cv->elem_size);
  memcpy(copy, data, cv->elem_size);
  cvector_push(cv, copy);
  while (cv->allocd_cap < cv->len) {
    cv->allocd_cap = (cv->allocd_cap == 0) ? 1 : cv->allocd_cap * 2;
    cv->allocd = (int *)s_realloc(cv->allocd, sizeof(int) * cv->allocd_cap);
  }
  cv->allocd[cv->len - 1] = 1;
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

    int atmp = cv->allocd[start];
    cv->allocd[start] = cv->allocd[end];
    cv->allocd[end] = atmp;

    start += 1;
    end -= 1;
  }
}

void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *)) {
  TODO("make the allocd array hold pointers to allocated items, not indices",
       stderr);
  for (size_t i = 1; i < cv->len; i++) {
    if (cv->allocd[i] != cv->allocd[0]) {
      PANIC("ERROR: all elements must either be all stack-allocated or all "
            "dynamically allocated",
            stderr);
    }
  }
  qsort(cv->data, cv->len / cv->elem_size, sizeof(void *), compar);
}

Cvector cvector_map(Cvector *cv, void (*map_func)(void *)) {
  Cvector mapped = cvector_create(cv->elem_size);
  for (size_t i = 0; i < cv->len; i++) {
    void *elem = cv->data[i];
    map_func(elem);
    cv->allocd[i] ? cvector_pushdyn(&mapped, cv->data[i])
                  : cvector_push(&mapped, cv->data[i]);
  }
  cvector_free(cv);
  return mapped;
}

void *cvector_peek(Cvector *cv) { return cv->data[cv->len - 1]; }

void cvector_pop(Cvector *cv) {
  if (!cv->len) {
    PANIC("called pop on a cvector with len = 0", stderr);
  }
  if (cv->allocd[cv->len - 1]) {
    free(cv->data[cv->len - 1]);
    cv->allocd[cv->len - 1] = 0;
  }
  cv->len -= 1;
}

void *cvector_at(Cvector *cv, size_t idx) {
  if (idx >= cv->len) {
    return NULL;
  }
  return cv->data[idx];
}

void cvector_clear(Cvector *cv) { cv->len = 0; }

size_t cvector_len(Cvector *cv) { return cv->len / cv->elem_size; }

size_t cvector_cap(Cvector *cv) { return cv->cap; }

int cvector_empty(Cvector *cv) { return !cv->len; }

void cvector_free(Cvector *cv) {
  for (size_t i = 0; i < cv->len; i++) {
    if (cv->allocd[i]) {
      free(cv->data[i]);
    }
  }
  cv->len = cv->cap = cv->allocd_cap = 0;
  if (cv->data) {
    free(cv->data);
  }
  if (cv->allocd) {
    free(cv->allocd);
  }
  cv->data = NULL;
  cv->allocd = NULL;
}
