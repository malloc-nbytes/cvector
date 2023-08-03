#ifndef CVECTOR_H
#define CVECTOR_H

#include <stdlib.h>

typedef struct {
  void *data;
  size_t stride, len, cap;
} Vec;

#define vec_push_const(vec, x) (vec_push(vec, (void *)&(typeof(x)){(x)}))

Vec vec_new(size_t stride);
void vec_push(Vec *vec, void *data);
void *vec_at(Vec *vec, size_t idx);
Vec vec_map(Vec *vec, void *(*mapfunc)(void *elem), size_t stride);
void vec_iter(Vec *vec, void (*pure_procedure)(void *elem));
void vec_free(Vec *vec);

#endif // CVECTOR_H
