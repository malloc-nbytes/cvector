#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>
#include <string.h>

#define CVQSORT_COMPARFUNC_CAST(x, type) *((type *)*((void **)(x)))

#define CVECTOR_PRINT(cv, type, format)                                        \
  do {                                                                         \
    for (size_t i = 0; i < (cv)->len; i++) {                                   \
      if (strcmp(#type, "char *") == 0) {                                      \
        printf("%s\n", (char *)((cv)->data[i]));                               \
      } else {                                                                 \
        printf(#format "\n", *((type *)((cv)->data[i])));                      \
      }                                                                        \
    }                                                                          \
  } while (0)

#define CV_COMPOUND_LITERAL(x) ((void *)&(typeof(x)){x})
#define EXPLICIT_CV_COMPOUND_LITERAL(x, type) ((void *)&(type){x})

#define CVWRAP(x) CV_COMPOUND_LITERAL(x)
#define ECVWRAP(x, type) EXPLICIT_CV_COMPOUND_LITERAL(x, type)

typedef struct {
  void **data;
  size_t len, cap, elem_size;
  int *allocd;
  size_t allocd_len, allocd_cap;
} Cvector;

Cvector cvector_create(size_t elem_size);
Cvector cvector_with_capacity(size_t cap, size_t elem_size);

// Consumes the vector.
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));

void cvector_push(Cvector *cv, void *data);
void cvector_pushvar(Cvector *cv, void *data, size_t elem_size);
void cvector_rev(Cvector *cv);
void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *));
void cvector_clear(Cvector *cv);
void cvector_free(Cvector *cv);

void *cvector_pop(Cvector *cv);
void *cvector_at(Cvector *cv, size_t idx);
void *cvector_peek(Cvector *cv);

// Consumes the vector.
void *cvector_fold_right(Cvector *cv, void (*func)(void *, void *));

size_t cvector_len(Cvector *cv);
size_t cvector_cap(Cvector *cv);

int cvector_empty(Cvector *cv);

#endif // CVECTOR_H
