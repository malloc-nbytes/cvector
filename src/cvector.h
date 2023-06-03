#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>

// (ﾉಥДಥ)ﾉ ︵┻━┻･/
#define CV_COMPOUND_LITERAL(x) ((void *)&(typeof(x)){(x)})
#define CV_EXPLICIT_COMPOUND_LITERAL(x, type) ((void *)&(type){(x)})
#define CVCONST(x) CV_COMPOUND_LITERAL(x)
#define ECVCONST(x, type) CV_EXPLICIT_COMPOUND_LITERAL(x, type)

#define CVCREATE(type) cvector_create(sizeof(type))
#define CVWITHCAP(cap, type) cvector_with_capacity(cap, sizeof(type))

#define CVPRINT(cv, type, format) do { \
  for (size_t i = 0; i < cv.len; i++) { \
    printf(format "\n", ((type *)cv.data)[i]); \
  } \
} while (0)

typedef struct {
  void *data;
  size_t len;
  size_t cap;
  size_t elem_size;
} Cvector;

// Create a new `Cvector`.
Cvector cvector_create(size_t elem_size);

// Create a new `Cvector` with a capacity.
Cvector cvector_with_capacity(size_t cap, size_t elem_size);

void cvector_remove(Cvector *cv, size_t index);

// Create a new `Cvector` by applying a function
// to each element. This function consumes the vector.
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));

// Set the length of the cvector. `len` <= |`cv`|
void cvector_set_len(Cvector *cv, size_t len);

// Apply an immutable function to each element.
void cvector_foreach(Cvector *cv, void (foreach_func)(const void *));

// Push a reference to a variable or push
// a compound literal using one of the following:
//  CVWRAP
//  ECVWRAP
//  CV_COMPOUND_LITERAL
//  EXPLICIT_CV_COMPOUND_LITERAL
void cvector_push(Cvector *cv, void *data);

// Reverse the `Cvector`.
void cvector_rev(Cvector *cv);

// Apply qsort() on the `Cvector`.
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

// Apply cv[0] = func(cv[0], cv[1]), func(cv[0], cv[2]), ..., func(cv[0], cv[n-1]).
// The length of `cv` will be set to 1.
void cvector_fold_right(Cvector *cv, void (*func)(void *, void *));

// Get the length.
size_t cvector_len(Cvector *cv);

// Get the element size.
size_t cvector_elem_size(Cvector *cv);

// Get the capacity.
size_t cvector_cap(Cvector *cv);

// Check to see if the `Cvector` is empty.
int cvector_empty(Cvector *cv);

// Check to see if the `Cvector` contains an element.
int cvector_contains(Cvector *cv, void *elem);

// Count the number of occurrences of an element.
int cvector_count(Cvector *cv, void *elem);

// Combine two vectors into one.
void cvector_extend(Cvector *dest, Cvector *release);

// Filter the vector in place.
void cvector_inplace_filter(Cvector *cv, int (*inplace_filter_func)(const void *));

// Filter the vector and return a new vector.
Cvector cvector_filter(Cvector *cv, int (*filter_func)(const void *));

// Check to see if both vectors have the same values somewhere in memory.
// v1 = [1, 2, 3]
// v2 = [1, 2, 3]
// v1 == v2 => true
// v1 = [1, 2, 3]
// v2 = [1, 3, 2]
// v1 == v2 => true
// v1 = [1, 2, 3]
// v2 = [1, 3, 2, 4]
// v1 == v2 => false
int cvector_shallow_eq(Cvector *cv1, Cvector *cv2);

// Check to see if every element (in order) in the vectors are equal.
// v1 = [1, 2, 3]
// v2 = [1, 2, 3]
// v1 == v2 => true, reason: same values, same order.
// v1 = [1, 2, 3]
// v2 = [1, 3, 2]
// v1 == v2 => false, reason: same values, different order.
// v1 = [1, 2, 3]
// v2 = [1, 3, 2, 4]
// v1 == v2 => false, reason: |v1| != |v2|.
int cvector_deep_eq(Cvector *cv1, Cvector *cv2);

// Return a new vector with the same elements as the original vector.
Cvector cvector_copy(Cvector *cv);

// Return the index of the first occurrence of a given element in the vector.
int cvector_index(Cvector *cv, void *elem);

// Change a value at an index.
void cvector_change(Cvector *cv, int index, void *new_data);

#endif // CVECTOR_H
