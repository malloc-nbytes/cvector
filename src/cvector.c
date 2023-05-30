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

Cvector cvector_with_capacity(size_t cap, size_t elem_size) {
  Cvector cv;

  cv.elem_size = elem_size;
  cv.len = 0;
  cv.cap = cap;

  cv.data = (void **)_cvector_malloc(sizeof(void *) * cap);
  cv.allocated = (int *)_cvector_malloc(sizeof(int) * cap);

  memset(cv.allocated, 0, sizeof(cv.allocated[0]) * cap);

  return cv;
}

Cvector cvector_create(size_t elem_size) {
  Cvector cv;

  cv.data = NULL;
  cv.elem_size = elem_size;
  cv.len = cv.cap = 0;

  cv.allocated = NULL;

  return cv;
}

void cvector_push(Cvector *cv, void *data) {
  if (cv->len >= cv->cap) {
    cv->cap = (cv->cap == 0) ? 1 : cv->cap * 2;

    cv->data = (void **)_cvector_realloc(cv->data, sizeof(void *) * cv->cap);
    cv->allocated = (int *)_cvector_realloc(cv->allocated, sizeof(int) * cv->cap);

    size_t new_elements = (cv->cap - cv->cap) * sizeof(int);
    memset(cv->allocated + cv->cap, 0, new_elements);
  }
  cv->data[cv->len++] = data;
  cv->allocated[cv->len - 1] = 0;
}

void cvector_pushdyn(Cvector *cv, void *data) {
  void *copy = _cvector_malloc(cv->elem_size);
  memcpy(copy, data, cv->elem_size);
  cvector_push(cv, copy);
  if (cv->cap < cv->len) {
    cv->cap = (cv->cap == 0) ? 1 : cv->cap * 2;
    cv->allocated = (int *)_cvector_realloc(cv->allocated, sizeof(int) * cv->cap);
  }
  cv->allocated[cv->len - 1] = 1;
}

// Places the result at index 0 of the vector.
// Example `func`:
// void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }
void *cvector_fold_right(Cvector *cv, void (*func)(void *, void *)) {
  assert(cv->len > 2);
  for (size_t i = 0; i < cv->len - 1; i++) {
    func(cv->data[cv->len - 2 - i], cv->data[cv->len - 1 - i]);
  }
  return cv->data[0];
}

void cvector_rev(Cvector *cv) {
  int start = 0, end = cv->len - 1;
  while (start < end) {
    void *tmp = cv->data[start];
    cv->data[start] = cv->data[end];
    cv->data[end] = tmp;

    int atmp = cv->allocated[start];
    cv->allocated[start] = cv->allocated[end];
    cv->allocated[end] = atmp;

    start += 1;
    end -= 1;
  }
}

// Example compar func:
/*
int compar(const void *a, const void *b) {
  int n1 = CVQSORT_COMPARFUNC_CAST(a, int);
  int n2 = CVQSORT_COMPARFUNC_CAST(b, int);
  if (n1 < n2) {
    return -1;
  } else if (n1 > n2) {
    return 1;
  } else {
    return 0;
  }
}
*/
void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *)) {
  for (size_t i = 1; i < cv->len; i++) {
    if (cv->allocated[i] != cv->allocated[0]) {
      PANIC("ERROR: all elements must either be all stack-allocated or all "
            "dynamically allocated",
            stderr);
    }
  }
  qsort(cv->data, cv->len / cv->elem_size, sizeof(void *), compar);
}

// Consumes the `Cvector`.
// Example `map_func`:
// void mult_by_two(void *elem) { *(int *)elem *= 2; }
Cvector cvector_map(Cvector *cv, void (*map_func)(void *)) {
  Cvector mapped = cvector_create(cv->elem_size);
  for (size_t i = 0; i < cv->len; i++) {
    void *elem = cv->data[i];
    map_func(elem);
    cv->allocated[i] ? cvector_pushdyn(&mapped, cv->data[i])
                     : cvector_push(&mapped, cv->data[i]);
  }
  cvector_free(cv);
  return mapped;
}

void cvector_foreach(Cvector *cv, void (foreach_func)(const void *)) {
  for (size_t i = 0; i < cv->len; i++) {
    foreach_func(cv->data[i]);
  }
}

void *cvector_peek(Cvector *cv) {
  return cv->data[cv->len - 1];
}

void try_free_individual_elem(Cvector *cv, size_t idx) {
  if (cv->allocated[idx]) {
    free(cv->data[idx]);
    cv->allocated[idx] = 0;
  }
}

void cvector_pop(Cvector *cv) {
  if (!cv->len) {
    PANIC("called pop on a cvector with len = 0", stderr);
  }
  try_free_individual_elem(cv, cv->len - 1);
  cv->len -= 1;
}

void *cvector_at(Cvector *cv, size_t idx) {
  if (idx >= cv->len) {
    return NULL;
  }
  return cv->data[idx];
}

void cvector_clear(Cvector *cv) { cvector_free(cv); }

size_t cvector_len(Cvector *cv) { return cv->len; }

size_t cvector_cap(Cvector *cv) { return cv->cap; }

int cvector_empty(Cvector *cv) { return !cv->len; }

void cvector_free(Cvector *cv) {
  for (size_t i = 0; i < cv->len; i++) {
    if (cv->allocated[i]) {
      free(cv->data[i]);
    }
  }
  cv->len = cv->cap = 0;
  if (cv->data) {
    free(cv->data);
  }
  if (cv->allocated) {
    free(cv->allocated);
  }
  cv->data = NULL;
  cv->allocated = NULL;
}
