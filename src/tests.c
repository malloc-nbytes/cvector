#include "cvector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_cvector_create() {
  Cvector cv = cvector_create(sizeof(int));
  assert(cv.data == NULL);
  assert(cv.len == 0);
  assert(cv.cap == 0);
  assert(cv.elem_size == sizeof(int));
  cvector_free(&cv);
  printf("(+) test_cvector_create\n");
}

void test_cvector_with_capacity() {
  Cvector cv = cvector_with_capacity(10, sizeof(int));
  assert(cv.data != NULL);
  assert(cv.len == 0);
  assert(cv.cap == 10);
  assert(cv.elem_size == sizeof(int));
  cvector_free(&cv);
  printf("(+) test_cvector_with_capacity\n");
}

void test_cvector_push() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  assert(cv.len == 3);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  assert(*(int *)cvector_at(&cv, 1) == 2);
  assert(*(int *)cvector_at(&cv, 2) == 3);
  cvector_free(&cv);
  printf("(+) test_cvector_push\n");
}

void test_cvector_pop() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_pop(&cv);
  assert(cv.len == 2);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  assert(*(int *)cvector_at(&cv, 1) == 2);
  cvector_pop(&cv);
  assert(cv.len == 1);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  cvector_pop(&cv);
  assert(cv.len == 0);
  cvector_free(&cv);
  printf("(+) test_cvector_pop\n");
}

void test_cvector_remove() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_remove(&cv, 1);
  assert(cv.len == 2);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  assert(*(int *)cvector_at(&cv, 1) == 3);
  cvector_remove(&cv, 0);
  assert(cv.len == 1);
  assert(*(int *)cvector_at(&cv, 0) == 3);
  cvector_remove(&cv, 0);
  assert(cv.len == 0);
  cvector_free(&cv);
  printf("(+) test_cvector_remove\n");
}

void test_cvector_peek() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  assert(cvector_peek(&cv) == NULL);
  cvector_push(&cv, &a);
  assert(*(int *)cvector_peek(&cv) == 1);
  cvector_push(&cv, &b);
  assert(*(int *)cvector_peek(&cv) == 2);
  cvector_push(&cv, &c);
  assert(*(int *)cvector_peek(&cv) == 3);
  cvector_pop(&cv);
  assert(*(int *)cvector_peek(&cv) == 2);
  cvector_pop(&cv);
  assert(*(int *)cvector_peek(&cv) == 1);
  cvector_pop(&cv);
  assert(cvector_peek(&cv) == NULL);
  cvector_free(&cv);
  printf("(+) test_cvector_peek\n");
}

void test_cvector_foreach() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  int sum = 0;
  void sum_func(const void *elem) { sum += *(int *)elem; }
  cvector_foreach(&cv, sum_func);
  assert(sum == 6);
  cvector_free(&cv);
  printf("(+) test_cvector_foreach\n");
}

void test_cvector_set_len() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_set_len(&cv, 2);
  assert(cv.len == 2);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  assert(*(int *)cvector_at(&cv, 1) == 2);
  cvector_set_len(&cv, 1);
  assert(cv.len == 1);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  cvector_set_len(&cv, 0);
  assert(cv.len == 0);
  cvector_free(&cv);
  printf("(+) test_cvector_set_len\n");
}

void test_cvector_fold_right() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  void sum_func(void *acc, void *elem) { *(int *)acc += *(int *)elem; }
  cvector_fold_right(&cv, sum_func);
  assert(cv.len == 1);
  assert(*(int *)cvector_at(&cv, 0) == 10);
  cvector_free(&cv);
  printf("(+) test_cvector_fold_right\n");
}

void test_cvector_qsort() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 3, b = 1, c = 4, d = 2;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  int cmp(const void *a, const void *b) { return *(int *)a - *(int *)b; }
  cvector_qsort(&cv, cmp);
  assert(cv.len == 4);
  assert(*(int *)cvector_at(&cv, 0) == 1);
  assert(*(int *)cvector_at(&cv, 1) == 2);
  assert(*(int *)cvector_at(&cv, 2) == 3);
  assert(*(int *)cvector_at(&cv, 3) == 4);
  cvector_free(&cv);
  printf("(+) test_cvector_qsort\n");
}

void test_cvector_map() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  void double_func(void *elem) { *(int *)elem *= 2; }
  Cvector mapped = cvector_map(&cv, double_func);
  assert(cv.len == 0);
  assert(mapped.len == 3);
  assert(*(int *)cvector_at(&mapped, 0) == 2);
  assert(*(int *)cvector_at(&mapped, 1) == 4);
  assert(*(int *)cvector_at(&mapped, 2) == 6);
  cvector_free(&cv);
  cvector_free(&mapped);
  printf("(+) test_cvector_map\n");
}

void test_cvector_rev() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  cvector_rev(&cv);
  assert(cv.len == 4);
  assert(*(int *)cvector_at(&cv, 0) == 4);
  assert(*(int *)cvector_at(&cv, 1) == 3);
  assert(*(int *)cvector_at(&cv, 2) == 2);
  assert(*(int *)cvector_at(&cv, 3) == 1);
  cvector_free(&cv);
  printf("(+) test_cvector_rev\n");
}

void test_cvector_clear() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_clear(&cv);
  assert(cv.len == 0);
  cvector_free(&cv);
  printf("(+) test_cvector_clear\n");
}

void test_cvector_len() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3;
  assert(cvector_len(&cv) == 0);
  cvector_push(&cv, &a);
  assert(cvector_len(&cv) == 1);
  cvector_push(&cv, &b);
  assert(cvector_len(&cv) == 2);
  cvector_push(&cv, &c);
  assert(cvector_len(&cv) == 3);
  cvector_pop(&cv);
  assert(cvector_len(&cv) == 2);
  cvector_pop(&cv);
  assert(cvector_len(&cv) == 1);
  cvector_pop(&cv);
  assert(cvector_len(&cv) == 0);
  cvector_free(&cv);
  printf("(+) test_cvector_len\n");
}

void test_cvector_empty() {
  Cvector cv = cvector_create(sizeof(int));
  assert(cvector_empty(&cv));
  int a = 1;
  cvector_push(&cv, &a);
  assert(!cvector_empty(&cv));
  cvector_pop(&cv);
  assert(cvector_empty(&cv));
  cvector_free(&cv);
  printf("(+) test_cvector_empty\n");
}

void test_cvector_count() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 2;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  assert(cvector_count(&cv, &a) == 1);
  assert(cvector_count(&cv, &b) == 2);
  assert(cvector_count(&cv, &c) == 1);
  assert(cvector_count(&cv, &d) == 2);
  cvector_free(&cv);
  printf("(+) test_cvector_count\n");
}

void test_cvector_extend() {
  Cvector cv1 = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv1, &a);
  cvector_push(&cv1, &b);
  cvector_push(&cv2, &c);
  cvector_push(&cv2, &d);
  cvector_extend(&cv1, &cv2);
  assert(cv1.len == 4);
  assert(*(int *)cvector_at(&cv1, 0) == 1);
  assert(*(int *)cvector_at(&cv1, 1) == 2);
  assert(*(int *)cvector_at(&cv1, 2) == 3);
  assert(*(int *)cvector_at(&cv1, 3) == 4);
  cvector_free(&cv1);
  cvector_free(&cv2);
  printf("(+) test_cvector_extend\n");
}

void test_cvector_inplace_filter() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  int is_even(const void *elem) {
    return *(int *)elem % 2 == 0;
  }
  cvector_inplace_filter(&cv, is_even);
  assert(cv.len == 2);
  assert(*(int *)cvector_at(&cv, 0) == 2);
  assert(*(int *)cvector_at(&cv, 1) == 4);
  cvector_free(&cv);
  printf("(+) test_cvector_inplace_filter\n");
}

void test_cvector_filter() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  int is_even(const void *elem) {
    return *(int *)elem % 2 == 0;
  }
  Cvector cv2 = cvector_filter(&cv, is_even);
  assert(cv2.len == 2);
  assert(*(int *)cvector_at(&cv2, 0) == 2);
  assert(*(int *)cvector_at(&cv2, 1) == 4);
  cvector_free(&cv);
  cvector_free(&cv2);
  printf("(+) test_cvector_filter\n");
}

void test_cvector_shallow_eq() {
  Cvector cv1 = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv1, &a);
  cvector_push(&cv1, &b);
  cvector_push(&cv2, &a);
  cvector_push(&cv2, &b);
  assert(cvector_shallow_eq(&cv1, &cv2));
  cvector_rev(&cv2);
  assert(cvector_shallow_eq(&cv1, &cv2));
  cvector_push(&cv2, &c);
  assert(!cvector_shallow_eq(&cv1, &cv2));
  cvector_free(&cv1);
  cvector_free(&cv2);
  printf("(+) test_cvector_shallow_eq\n");
}

void test_cvector_deep_eq() {
  Cvector cv1 = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv1, &a);
  cvector_push(&cv1, &b);
  cvector_push(&cv2, &a);
  cvector_push(&cv2, &b);
  assert(cvector_deep_eq(&cv1, &cv2));
  cvector_rev(&cv2);
  assert(!cvector_deep_eq(&cv1, &cv2));
  cvector_free(&cv1);
  cvector_free(&cv2);
  printf("(+) test_cvector_deep_eq\n");
}

void test_cvector_copy() {
  Cvector cv1 = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv1, &a);
  cvector_push(&cv1, &b);
  cvector_push(&cv1, &c);
  cvector_push(&cv1, &d);
  Cvector cv2 = cvector_copy(&cv1);
  assert(cvector_shallow_eq(&cv1, &cv2));
  cvector_free(&cv1);
  cvector_free(&cv2);
  printf("(+) test_cvector_copy\n");
}

void test_cvector_index() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  assert(cvector_index(&cv, &a) == 0);
  assert(cvector_index(&cv, &b) == 1);
  assert(cvector_index(&cv, &c) == 2);
  assert(cvector_index(&cv, &d) == 3);
  assert(cvector_index(&cv, CVCONST(9)) == -1);
  cvector_free(&cv);
  printf("(+) test_cvector_index\n");
}

void test_cvector_change() {
  Cvector cv = cvector_create(sizeof(int));
  int a = 1, b = 2, c = 3, d = 4;
  cvector_push(&cv, &a);
  cvector_push(&cv, &b);
  cvector_push(&cv, &c);
  cvector_push(&cv, &d);
  int e = 5;
  cvector_change(&cv, 2, &e);
  assert(*(int *)cvector_at(&cv, 2) == 5);
  cvector_free(&cv);
  printf("(+) test_cvector_change\n");
}

int main() {
  test_cvector_create();
  test_cvector_with_capacity();
  test_cvector_push();
  test_cvector_pop();
  test_cvector_remove();
  test_cvector_peek();
  test_cvector_foreach();
  test_cvector_set_len();
  test_cvector_fold_right();
  test_cvector_qsort();
  test_cvector_map();
  test_cvector_rev();
  test_cvector_clear();
  test_cvector_len();
  test_cvector_empty();
  test_cvector_extend();
  test_cvector_inplace_filter();
  test_cvector_filter();
  test_cvector_shallow_eq();
  test_cvector_deep_eq();
  test_cvector_copy();
  test_cvector_index();
  test_cvector_change();
  printf("All tests passed!\n");
  return 0;
}
