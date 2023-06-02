# cvector

<img src="https://img.shields.io/badge/C-A8B9CC?logo=c&logoColor=black&style=for-the-badge" height="40" alt="c logo"  />
<img width="12" />

## A generic, growable array.

## Functions
### `cvector_create`
```c
Cvector cvector_create(size_t elem_size);
```
Creates a new cvector with the given element size.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));
  return 0;
}
```

### `cvector_with_capacity`
```c
Cvector cvector_with_capacity(size_t cap, size_t elem_size);
```
Create a new cvector with a capacity.

Example:
```c
int main(void) {
  Cvector cv = cvector_with_capacity(10, sizeof(int));
  return 0;
}
```

### `cvector_free`
```c
void cvector_free(Cvector *cv);
```
Frees the given cvector.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));
  cvector_free(&cv);
  return 0;
}
```

### `cvector_push`
```c
void cvector_push(Cvector *cv, void *data);
```
Pushes the given element onto the end of the cvector.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  int elem = 0;
  cvector_push(&cv, &elem);

  for (int i = 1; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  CVPRINT(&cv, int, "%d");

  cvector_free(&cv);

  return 0;
}
```
Output:
```
0
1
2
3
4
```

### `cvector_remove`
```c
void cvector_remove(Cvector *cv, size_t index);
```
Remove an element at an index.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  cvector_remove(&cv, 1);

  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
0
2
3
4
```

### `cvector_map`
```c
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));
```
Create a new cvector by applying a function to each element. This function consumes the original cvector.

Example:
```c
void elem_double(void *ptr) {
  int *elem = (int *)ptr;
  *elem *= 2;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  Cvector mapped = cvector_map(&cv, elem_double);

  CVPRINT(mapped, int, "%d");

  cvector_free(&mapped);
  return 0;
}
```
Output:
```
0
2
4
6
8
```

### `cvector_set_len`
```c
void cvector_set_len(Cvector *cv, size_t len);
```
Set the length of the cvector. `len` <= |`cv`|

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_set_len(&cv, 3);

  printf("After:\n");
  CVPRINT(cv, int, "%d");
 
  cvector_free(&cv);
  return 0;
}
```
Output:
```
Before:
0
1
2
3
4
After:
0
1
2
```

### `cvector_foreach`
```c
void cvector_foreach(Cvector *cv, void (foreach_func)(const void *));
```
Apply an immutable function to each element.

Example:
```c
void print(const void *ptr) {
  printf("%d\n", *(int *)ptr);
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  cvector_foreach(&cv, print);

  return 0;
}
```
Output:
```
0
1
2
3
4
```

### `cvector_rev`
```c
void cvector_rev(Cvector *cv);
```
Reverse the cvector.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 4; i >= 0; i--) {
    cvector_push(&cv, &i);
  }

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_rev(&cv);

  printf("After:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
Before:
4
3
2
1
0
After:
0
1
2
3
4
```

### `cvector_qsort`
```c
void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *));
```
Sort using qsort().

Example:
```c
int compar(const void *ptr1, const void *ptr2) {
  return *(int *)ptr1 - *(int *)ptr2;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  cvector_push(&cv, CVCONST(8));
  cvector_push(&cv, CVCONST(3));
  cvector_push(&cv, CVCONST(4));
  cvector_push(&cv, CVCONST(1));

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_qsort(&cv, compar);

  printf("After:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
Before:
8
3
4
1
After:
1
3
4
8
```

### `cvector_clear`
```c
void cvector_clear(Cvector *cv);
```
Clear the cvector.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_clear(&cv);

  printf("After:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
Before:
0
1
2
3
4
After:
```

### `cvector_pop`
```c
void cvector_pop(Cvector *cv);
```
Remove the last element.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("Before:\n");
  CVPRINT(cv, int, "%d");

  cvector_pop(&cv);

  printf("After:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
0
1
2
3
4
After:
0
1
2
3
```

### `cvector_at`
```c
void *cvector_at(Cvector *cv, size_t idx);
```
Retrieve a value at an index.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("%d\n", *(int *)cvector_at(&cv, 3));

  cvector_free(&cv);
  return 0;
}
```
Output:
```
3
```

### `cvector_peek`
```c
void *cvector_peek(Cvector *cv);
```
Get the element in the last position.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("%d\n", *(int *)cvector_peek(&cv));

  cvector_free(&cv);
  return 0;
}
```
Output:
```
4
```

### `cvector_fold_right`
```c
void cvector_fold_right(Cvector *cv, void (*func)(void *, void *));
```

Apply cv\[0] = func(cv\[0], cv\[1]), func(cv\[0], cv\[2]), ..., func(cv\[0], cv\[n-1]). The length of `cv` will be set to 1.

Example:
```c
void foldfunc(void *a, void *b) {
  int *ax = (int *)a;
  int *bx = (int *)b;
  *ax = *ax + *bx;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  cvector_fold_right(&cv, foldfunc);

  CVPRINT(cv, int, "%d");

  assert(cvector_len(&cv) == 1);

  cvector_free(&cv);
  return 0;
}
```
Output:
```
10
```

### `cvector_len`
```c
size_t cvector_len(Cvector *cv);
```
Get the length.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
    assert(cvector_len(&cv) == i + 1);
  }

  cvector_free(&cv);
  return 0;
}
```

### `cvector_elem_size`
```c
size_t cvector_elem_size(Cvector *cv);
```
Get the element size.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));
  assert(cvector_elem_size(&cv) == sizeof(int));
  cvector_free(&cv);
  return 0;
}
```

### `cvector_cap`
```c
size_t cvector_cap(Cvector *cv);
```
Get the capacity.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 1000000; i++) {
    cvector_push(&cv, &i);
  }

  printf("%zu\n", cvector_cap(&cv));

  cvector_free(&cv);
  return 0;
}
```
Output:
```
1048576
```

### `cvector_empty`
```c
int cvector_empty(Cvector *cv);
```
Check to see if the cvector is empty.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  assert(cvector_empty(&cv));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  assert(!cvector_empty(&cv));

  cvector_free(&cv);
  return 0;
}
```

### `cvector_contains`
```c
int cvector_contains(Cvector *cv, void *elem);
```
Check to see if the `Cvector` contains an element.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  printf("%s\n",
         cvector_contains(&cv, CVCONST(2))
         ? "true" : "false");
  printf("%s\n",
         cvector_contains(&cv, CVCONST(9))
         ? "true" : "false");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
true
false
```

### `cvector_count`
```c
int cvector_count(Cvector *cv, void *elem);
```
Count the number of occurrences of an element.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  cvector_push(&cv, CVCONST(1));
  cvector_push(&cv, CVCONST(3));
  cvector_push(&cv, CVCONST(1));
  cvector_push(&cv, CVCONST(1));
  cvector_push(&cv, CVCONST(3));
  cvector_push(&cv, CVCONST(9));

  printf("%d\n", cvector_count(&cv, CVCONST(1)));
  printf("%d\n", cvector_count(&cv, CVCONST(3)));
  printf("%d\n", cvector_count(&cv, CVCONST(9)));

  cvector_free(&cv);
  return 0;
}
```
Output:
```
3
2
1
```

### `cvector_extend`
```c
int cvector_extend(Cvector *dest, Cvector *release);
```
Combine two vectors into one. This function consumes `release` and places its elements into `dest`.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
    cvector_push(&cv2, CVCONST(i + 100));
  }

  printf("cv:\n");
  CVPRINT(cv, int, "%d");
  printf("cv2:\n");
  CVPRINT(cv2, int, "%d");

  cvector_extend(&cv, &cv2);

  printf("cv after extend:\n");
  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
cv:
0
1
2
3
4
cv2:
100
101
102
103
104
cv after extend:
0
1
2
3
4
100
101
102
103
104
```

### `cvector_inplace_filter`
```c
void cvector_inplace_filter(Cvector *cv, int (*inplace_filter_func)(const void *));
```
Filter the vector in place.

Example:
```c
int inplace_filter_func(const void *a) {
  return *(int *)a % 2 == 0;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  cvector_inplace_filter(&cv, inplace_filter_func);

  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
0
2
4
```

### `cvector_filter`
```c
Cvector cvector_filter(Cvector *cv, int (*filter_func)(const void *));
```
Filter the vector and return a new vector.

Example:
```c
int filter_func(const void *a) {
  return *(int *)a % 2 == 0;
}

int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  Cvector filtered = cvector_filter(&cv, filter_func);

  CVPRINT(filtered, int, "%d");

  cvector_free(&filtered);
  return 0;
}
```
Output:
```
0
2
4
```

### `cvector_shallow_eq`
```c
int cvector_shallow_eq(Cvector *cv1, Cvector *cv2);
```
Check to see if both vectors have the same values somewhere in memory.

`v1 = [1, 2, 3]`

`v2 = [1, 2, 3]`

`v1 == v2 => true` (same values)

`v1 = [1, 2, 3]`

`v2 = [1, 3, 2]`

`v1 == v2 => true` (same values)

`v1 = [1, 2, 3]`

`v2 = [1, 3, 2, 4]`

`v1 == v2 => false` (different values)

Example:
```c
int main(void) {
  Cvector cv1 = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv1, &i);
  }

  for (int i = 4; i >= 0; i--) {
    cvector_push(&cv2, &i);
  }

  printf("%s\n",
         cvector_shallow_eq(&cv1, &cv2)
         ? "true" : "false");

  cvector_free(&cv1);
  cvector_free(&cv2);
  return 0;
}
```
Output:
```
true
```

### `cvector_deep_eq`
```c
int cvector_deep_eq(Cvector *cv1, Cvector *cv2);
```
Check to see if every element (in order) in the vectors are equal.

`v1 = [1, 2, 3]`

`v2 = [1, 2, 3]`

`v1 == v2 => true` (same values, same order)

`v1 = [1, 2, 3]`

`v2 = [1, 3, 2]`

`v1 == v2 => false` (different order)

`v1 = [1, 2, 3]`

`v2 = [1, 3, 2, 4]`

`v1 == v2 => false` (|v1| != |v2|)

Example:
```c
int main(void) {
  Cvector cv1 = cvector_create(sizeof(int));
  Cvector cv2 = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv1, &i);
  }

  for (int i = 4; i >= 0; i--) {
    cvector_push(&cv2, &i);
  }

  printf("%s\n",
         cvector_deep_eq(&cv1, &cv2)
         ? "true" : "false");

  cvector_clear(&cv2);

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv2, &i);
  }

  printf("%s\n",
         cvector_deep_eq(&cv1, &cv2)
         ? "true" : "false");

  cvector_free(&cv1);
  cvector_free(&cv2);
  return 0;
}
```
Output:
```
false
true
```

### `cvector_copy`
```c
Cvector cvector_copy(Cvector *cv);
```
Return a new vector with the same elements as the original vector.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  Cvector copy = cvector_copy(&cv);

  printf("Original:\n");
  CVPRINT(cv, int, "%d");
  printf("Copy:\n");
  CVPRINT(copy, int, "%d");

  cvector_free(&cv);
  cvector_free(&copy);
  return 0;
}
```
Output:
```
Original:
0
1
2
3
4
Copy:
0
1
2
3
4
```

## Macros

| Macro                | Definition                   |
|----------------------|------------------------------|
| `CV_COMPOUND_LITERAL` | `((void *)&(typeof(x)){(x)})` |
| `CV_EXPLICIT_COMPOUND_LITERAL` | `((void *)&(type){(x)})` |
| `CVCONST` | `CV_COMPOUND_LITERAL(x)` |
| `ECVCONST` | `CV_EXPLICIT_COMPOUND_LITERAL(x, type)` |
| `CVCREATE` | `cvector_create(sizeof(type))` |
| `CVWITHCAP` | `cvector_with_capacity(cap, sizeof(type))` |
| `CVPRINT` | Try to print the `cvector` |

## Macro Examples

### `CVCONST` or `CV_COMPOUND_LITERAL`
`CVCONST(x)`, `CV_COMPOUND_LITERAL(x)`

Create a compound literal of a constant.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(int));

  cvector_push(&cv, CVCONST(1));
  cvector_push(&cv, CVCONST(2));
  cvector_push(&cv, CV_COMPOUND_LITERAL(3));

  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
1
2
3
```

### `ECVCONST` or `CV_EXPLICIT_COMPOUND_LITERAL`
`ECVCONST(x)`, `CV_EXPLICIT_COMPOUND_LITERAL(x)`

Create a compound literal of a constant and specify the type.

Example:
```c
int main(void) {
  Cvector cv = cvector_create(sizeof(float));

  cvector_push(&cv, ECVCONST(1.0, float));
  cvector_push(&cv, ECVCONST(2.0, float));
  cvector_push(&cv, CV_EXPLICIT_COMPOUND_LITERAL(3.0, float));

  CVPRINT(cv, float, "%f");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
1.000000
2.000000
3.000000
```

### `CVCREATE`
`CVCREATE(type)`

Wrapper for `cvector_create`.

Example:
```c
int main(void) {
  Cvector cv = CVCREATE(int);
  cvector_free(&cv);
  return 0;
}
```

### `CVWITHCAP`
`CVWITHCAP(cap, type)`

Wrapper for `cvector_with_capacity(cap, sizeof(type))`.

Example:
```c
int main(void) {
  Cvector cv = CVWITHCAP(10, int);
  cvector_free(&cv);
  return 0;
}
```

### `CVPRINT`
`CVPRINT(cv, type, format)`

Tries to print `cv`.

Example:
```c
int main(void) {
  Cvector cv = CVCREATE(int);

  for (int i = 0; i < 5; i++) {
    cvector_push(&cv, &i);
  }

  CVPRINT(cv, int, "%d");

  cvector_free(&cv);
  return 0;
}
```
Output:
```
0
1
2
3
4
```

