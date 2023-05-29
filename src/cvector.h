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

#define EXPLICIT_CV_COMPOUND_LITERAL(x, type) ((void *)&(type){x})
#define CV_COMPOUND_LITERAL(x) ((void *)&(typeof(x)){x})
#define CVWRAP(x) CV_COMPOUND_LITERAL(x)
#define ECVWRAP(x, type) EXPLICIT_CV_COMPOUND_LITERAL(x, type)

typedef struct {
  void **data;
  size_t len;
  size_t cap;
  size_t elem_size;
  int *allocd; // TODO: make this pointers instead of booleans.
  size_t allocd_cap; // TODO: remove this and just use cap.
} Cvector;

// Create a new `Cvector`.
Cvector cvector_create(size_t elem_size);

// Create a new `Cvector` with a capacity.
Cvector cvector_with_capacity(size_t cap, size_t elem_size);

// Create a new `Cvector` by applying a function
// to each element. This function consumes the vector.
// Example `map_func`:
//  void mult_by_two(void *elem) { *(int *)elem *= 2; }
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));

// Push a reference to a variable or push
// a compound literal using one of the following:
//  CVWRAP
//  ECVWRAP
//  CV_COMPOUND_LITERAL
//  EXPLICIT_CV_COMPOUND_LITERAL
void cvector_push(Cvector *cv, void *data);

// Push a value that will be dynamically allocated.
// Useful if there is a situation such as:
//  for (int i = 0; i < n; i++) {
//    cvector_pushdyn(&cv, &i);
//  }
void cvector_pushdyn(Cvector *cv, void *data);

// Reverse the `Cvector`.
void cvector_rev(Cvector *cv);

// Apply qsort() on the `Cvector`. Currently broken.
// Example `compar`:
//  void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }
void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *));

// Clear the `Cvector`.
void cvector_clear(Cvector *cv);

// Free the `Cvector`.
void cvector_free(Cvector *cv);

// Remove the last element.
void cvector_pop(Cvector *cv);

// Retrieve a value at an index.
void *cvector_at(Cvector *cv, size_t idx);

// Get the element in the last position.
void *cvector_peek(Cvector *cv);

// Apply func(cv_1, cv_2, cv_3, ..., cv_n-1).
// Consumes the vector.
// Example `func`:
//  void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }
void *cvector_fold_right(Cvector *cv, void (*func)(void *, void *));

// Get the length.
size_t cvector_len(Cvector *cv);

// Get the capacity.
size_t cvector_cap(Cvector *cv);

// Check to see if the `Cvector` is empty.
int cvector_empty(Cvector *cv);

#endif // CVECTOR_H
