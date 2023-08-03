#include "cvector.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define VECPANIC(msg, stream, ...)                                      \
  do {                                                                  \
    fprintf(stream, "PANIC [%s:%s:%d]: " msg "\n", __FILE__, __func__,  \
            __LINE__, ##__VA_ARGS__);                                   \
    exit(EXIT_FAILURE);                                                 \
  } while (0)


void *_vec_s_malloc(size_t bytes) {
  void *p = malloc(bytes);
  if (!p) {
    VECPANIC("Failed to allocate %zu bytes. Reason: %s", stderr, bytes, strerror(errno));
  }
}

Vec vec_new(size_t stride) {
  Vec v;
  v.data = malloc(stride);
  v.cap = 1;
  v.len = 0;
  v.stride = stride;
  return v;
}

void vec_push(Vec *vec, void *data) {
  while (vec->len >= vec->cap) {
    vec->cap *= 2;
    vec->data = realloc(vec->data, vec->cap * vec->stride);
  }
  (void)memcpy(vec->data + vec->len * vec->stride, data, vec->stride);
  vec->len += 1;
}

void *vec_at(Vec *vec, size_t idx) {
  if (idx >= vec->len) {
    VECPANIC("idx is >= vec->len", stderr);
  }
  return vec->data + idx * vec->stride;
}

Vec vec_map(Vec *vec, void *(*mapfunc)(void *elem), size_t stride) {
  Vec mapvec = vec_new(stride);
  for (size_t i = 0; i < vec->len; i++) {
    vec_push(&mapvec, mapfunc(vec_at(vec, i)));
  }
  vec_free(vec);
  return mapvec;
}

void vec_iter(Vec *vec, void (*pure_procedure)(void *elem)) {
  for (size_t i = 0; i < vec->len; i++) {
    closure(vec_at(vec, i));
  }
}

void vec_free(Vec *vec) {
  if (vec->data) {
    free(vec->data);
  }
  vec->len = vec->cap = 0;
}
