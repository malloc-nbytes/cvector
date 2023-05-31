# cvector
## A growable array capable of holding various datatypes.

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

TODO


