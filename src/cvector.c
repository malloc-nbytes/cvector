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

int is_allocd(Cvector *cv, size_t idx) {
  for (size_t i = 0; i < cv->allocd_len; i++) {
    if (cv->allocd[i] == idx) {
      return 1;
    }
  }
  return 0;
}

Cvector cvector_with_capacity(size_t cap, size_t elem_size) {
  Cvector cv;
  cv.elem_size = elem_size;
  cv.len = 0;
  cv.cap = cap;
  cv.data = (void **)s_malloc(sizeof(void *) * cap);
  cv.allocd = NULL;
  cv.allocd_len = cv.allocd_cap = 0;
  return cv;
}

Cvector cvector_create(size_t elem_size) {
  Cvector cv;
  cv.data = NULL;
  cv.elem_size = elem_size;
  cv.len = cv.cap = 0;
  cv.allocd = NULL;
  cv.allocd_len = cv.allocd_cap = 0;
  return cv;
}

void cvector_push(Cvector *cv, void *data) {
  if (cv->len >= cv->cap) {
    cv->cap = (cv->cap == 0) ? 1 : cv->cap * 2;
    cv->data = (void **)s_realloc(cv->data, sizeof(void *) * cv->cap);
  }
  cv->data[cv->len++] = data;
}

void cvector_pushvar(Cvector *cv, void *data, size_t elem_size) {
  void *copy = s_malloc(elem_size);
  memcpy(copy, data, elem_size);
  cvector_push(cv, copy);
  if (cv->allocd_len >= cv->allocd_cap) {
    cv->allocd_cap = (cv->allocd_cap == 0) ? 1 : cv->allocd_cap * 2;
    cv->allocd = (int *)s_realloc(cv->allocd, sizeof(int) * cv->allocd_cap);
  }
  cv->allocd[cv->allocd_len++] = cv->len - 1;
}

void *cvector_peek(Cvector *cv) { return cv->data[cv->len - 1]; }

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

    // The allocated indices get mixed up, resort them.
    for (size_t i = 0; i < cv->allocd_len; i++) {
      if (start == cv->allocd[i]) {
        cv->allocd[i] = end;
      } else if (end == cv->allocd[i]) {
        cv->allocd[i] = start;
      }
    }

    start += 1;
    end -= 1;
  }
}

void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *)) {
  qsort(cv->data, cv->len, sizeof(void *), compar);
}

Cvector cvector_map(Cvector *cv, void (*map_func)(void *)) {
  Cvector mapped = cvector_create(cv->elem_size);
  for (size_t i = 0; i < cv->len; i++) {
    void *elem = cv->data[i];
    map_func(elem);
    if (is_allocd(cv, i)) {
      cvector_pushvar(&mapped, cv->data[i], cv->elem_size);
    } else {
      cvector_push(&mapped, cv->data[i]);
    }
  }
  cvector_free(cv);
  return mapped;
}

void *cvector_pop(Cvector *cv) {
  assert(0 && "todo");
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

void cvector_clear(Cvector *cv) { cv->len = 0; }

size_t cvector_len(Cvector *cv) { return cv->len; }

size_t cvector_cap(Cvector *cv) { return cv->cap; }

int cvector_empty(Cvector *cv) { return !cv->len; }

void cvector_free(Cvector *cv) {
  for (size_t i = 0; i < cv->allocd_len; i++) {
    free(cv->data[cv->allocd[i]]);
  }
  cv->len = cv->cap = cv->allocd_len = cv->allocd_cap = 0;
  if (cv->data) {
    free(cv->data);
  }
  if (cv->allocd) {
    free(cv->allocd);
  }
  cv->data = NULL;
  cv->allocd = NULL;
}
