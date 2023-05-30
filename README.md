# cvector
## Remake of my now privated `vector` repository now using `void **` to hold data to reduce repetative code.

## Functions
#### * Create a new `Cvector`.
```c
Cvector cvector_create(size_t elem_size);
```
Example:
```c
Cvector cv = cvector_create(sizeof(int));
```

##### * Create a new `Cvector` with a capacity.
```c
Cvector cvector_with_capacity(size_t cap, size_t elem_size);
```
Example:
```c
Cvector cv = cvector_with_capacity(100, sizeof(int));
```

#### * Push a reference to a variable.
```c
void cvector_push(Cvector *cv, void *data);
```
Or push a compound literal using one of the following:

`CVWRAP` `ECVWRAP` `CV_COMPOUND_LITERAL` `EXPLICIT_CV_COMPOUND_LITERAL`

Example:
```c
Cvector cs = cvector_create(sizeof(float));
float a = 0.f;
cvector_push(&cv, &a);
cvector_push(&cv, CVWRAP(1.f));
cvector_push(&cv, ECVWRAP(2.0, float));
```

#### * Push a value that will be dynamically allocated.
```c
void cvector_pushdyn(Cvector *cv, void *data);
```
Example:
```c
Cvector cs = cvector_create(sizeof(int));
for (int i = 0; i < n; i++) {
  cvector_pushdyn(&cv, &i);
}
CVECTOR_PRINT(&cs, int, "%d");
```
Output:
```
"0"
"1"
"2"
"3"
...
"n-1"
```

#### * Create a new `Cvector` by applying a function to each element. This function consumes the vector.
```c
Cvector cvector_map(Cvector *cv, void (*map_func)(void *));
```
Example:
```c
void mult_by_two(void *elem) {
  *(int *)elem *= 2;
}

Cvector cs = cvector_create(sizeof(int));
cvector_push(&cv, CVWRAP(0));
cvector_push(&cv, CVWRAP(1));
cvector_push(&cv, CVWRAP(2));
Cvector mapped = cvector_map(&cv, mult_by_two);
CVECTOR_PRINT(&mapped, int, "%d");
cvector_free(&mapped);
```
Output:
```
"0""
"2""
"4""
```
Another example:
```c
void find_vowels(void *elem) {
  char *s = (char *)elem;
  for (int i = 0; s[i] != '\0'; i++) {
    char c = s[i];
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
      s[i] = 'x';
    }
  }
}

Cvector cv = cvector_create(sizeof(char *));

vector_push(&cv, CVWRAP("This is a test"));
cvector_push(&cv, CVWRAP("Hello world!"));
cvector_push(&cv, CVWRAP("Foo Bar Baz"));

CVECTOR_PRINT(&cv, char *, "%s");

vector mapped = cvector_map(&cv, find_vowels);
CVECTOR_PRINT(&mapped, char *, "%s");

cvector_free(&mapped);
```
Output:
```
This is a test
Hello world!
Foo Bar Baz
Thxs xs x txst
Hxllx wxrld!
Fxx Bxr Bxz
```

Calling `cvector_free()` on `cv` is no longer needed but is still needed for `mapped`.

#### * Reverse the `Cvector`.
```c
void cvector_rev(Cvector *cv);
```
Example:
```c
Cvector cs = cvector_create(sizeof(int));
for (int i = 0; i < 3; i++) {
  cvector_pushdyn(&cv, &i);
}
CVECTOR_PRINT(&cs, int, "%d");
```
Output:
```
"2"
"1"
"0"
```

#### * Apply qsort() on the `Cvector`. Currently broken.
```c
void cvector_qsort(Cvector *cv, int (*compar)(const void *, const void *));
```
Example:
```c
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

Cvector cs = cvector_create(sizeof(int));
for (int i = 2; i <= 0; i--) {
  cvector_pushdyn(&cv, &i);
}
cvector_qsort(&cv, compar);
CVECTOR_PRINT(&cs, int, "%d");
```
Output:
```
"0"
"1"
"2"
```

#### * Clear the `Cvector`.
```c
void cvector_clear(Cvector *cv);
```

#### * Free the `Cvector`.
```c
void cvector_free(Cvector *cv);
```

#### * Remove the last element.
```c
void cvector_pop(Cvector *cv);
```

#### * Retrieve a value at an index.
```c
void *cvector_at(Cvector *cv, size_t idx);
```
Example:
```c
Cvector cs = cvector_create(sizeof(int));
for (int i = 0; i < 5; i++) {
  cvector_pushdyn(&cv, &i);
}
int val = *(int *)cvector_at(&cv, 2);
printf("%d\n", val);
```
Output:
```
"2"
```

#### * Get the element in the last position.
```c
void *cvector_peek(Cvector *cv);
```
Example:
```c
Cvector cs = cvector_create(sizeof(int));
for (int i = 0; i < 5; i++) {
  cvector_pushdyn(&cv, &i);
}
int last = *(int *)cvector_peek(&cv);
printf("%d\n", last);
```
Output:
```
"4"
```

#### * Apply cv[0] = func(cv[0], cv[1], cv[2], ..., cv[n-1]).
```c
void *cvector_fold_right(Cvector *cv, void (*func)(void *, void *));
```
It will place the result at the first index of `cv` and also returns a pointer to it.
Example:
```c
void sum(void *a, void *b) { 
  *(int *)a = *(int *)a + *(int *)b;
}

Cvector cs = cvector_create(sizeof(int));
for (int i = 0; i < 5; i++) {
  cvector_pushdyn(&cv, &i);
}
printf("%d", *(int *)cvector_fold_right(&cv, sum));
// or
int res = cvector_fold_right(&cv, sum);
printf("%d", res);
```
Output:
```
"10"
```

#### * Get the length.
```c
size_t cvector_len(Cvector *cv);
```

#### * Get the capacity.
```c
size_t cvector_cap(Cvector *cv);
```

#### * Check to see if the `Cvector` is empty.
```c
int cvector_empty(Cvector *cv);
```
Example:
```c
while (!cvector_empty(&cv)) {
  cvector_pop(&cv);
}
```

## Macros
Create a compound literal.
| Macro                                   | Description                             |
|-----------------------------------------|-----------------------------------------|
| `CV_COMPOUND_LITERAL(x, type)`          | `((void *)&(typeof(x)){x})`             |
| `EXPLICIT_CV_COMPOUND_LITERAL(x, type)` | `((void *)&(type){x})`                  |
| `CVWRAP(x)`                             | `CV_COMPOUND_LITERAL(x)`                |
| `ECVWRAP(x, type)`                      | `EXPLICIT_CV_COMPOUND_LITERAL(x, type)` |
| `CVQSORT_COMPARFUNC_CAST(x, type)`      | `*((type *)*((void **)(x)))`            |
| `CVECTOR_PRINT(cv, type, format)`       | Try to print the `Cvector`              |

### Macro Examples
* `CVECTOR_PRINT`
```c
CVECTOR_PRINT(&cv, int, "%d");
```

* `CVWRAP(x)`
```c
Cvector cv = cvector_create();
cvector_push(&cv, CVWRAP(1));
cvector_push(&cv, CVWRAP(2));
cvector_push(&cv, CVWRAP(3));
```

* `ECVWRAP(x)`
```c
Cvector cv = cvector_create();
cvector_push(&cv, ECVWRAP(1.0, double));
cvector_push(&cv, ECVWRAP(2.0, double));
cvector_push(&cv, ECVWRAP(3.0, double));
```

* `CVQSORT_COMPARFUNC_CAST(x, type)`
```c
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
```


