#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>

typedef struct {
  void *data;
  size_t len;
  size_t cap;
  size_t elem_size;
} Cvector;

// Create a new `Cvector`.
Cvector cvector_create(size_t elem_size);

// Create a new `Cvector` with a capacity.
/* Cvector cvector_with_capacity(size_t cap, size_t elem_size); */

// Create a new `Cvector` by applying a function
// to each element. This function consumes the vector.
// Example `map_func`:
//  void mult_by_two(void *elem) { *(int *)elem *= 2; }
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));

void cvector_set_len(Cvector *cv, size_t len);

// Apply an immutable function to each element.
void cvector_foreach(Cvector *cv, void(foreach_func)(const void *));

// Push a reference to a variable or push
// a compound literal using one of the following:
//  CVWRAP
//  ECVWRAP
//  CV_COMPOUND_LITERAL
//  EXPLICIT_CV_COMPOUND_LITERAL
void cvector_push(Cvector *cv, void *data);

// Reverse the `Cvector`.
/* void cvector_rev(Cvector *cv); */

// Apply qsort() on the `Cvector`. Currently broken.
// Example `compar`:
//  void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }
/* void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *));
 */

// Clear the `Cvector`.
/* void cvector_clear(Cvector *cv); */

// Free the `Cvector`.
void cvector_free(Cvector *cv);

// Remove the last element.
void cvector_pop(Cvector *cv);

// Retrieve a value at an index.
void *cvector_at(Cvector *cv, size_t idx);

// Get the element in the last position.
void *cvector_peek(Cvector *cv);

// Apply cv[0] = func(cv[0], cv[1], cv[2], ..., cv[n-1]).
// It will place the result at the first index of cv and also returns a pointer
// to it. Example `func`:
//  void sum(void *a, void *b) { *(int *)a = *(int *)a + *(int *)b; }
void cvector_fold_right(Cvector *cv, void (*func)(void *, void *));

// Get the length.
size_t cvector_len(Cvector *cv);

// Get the capacity.
size_t cvector_cap(Cvector *cv);

// Check to see if the `Cvector` is empty.
int cvector_empty(Cvector *cv);

#endif // CVECTOR_H
