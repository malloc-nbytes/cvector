CLFAGS = -Wall -Wextra -pedantic -std=c99
HEADERS = cvector.h
OBJECTS = cvector.o

.PHONY: clean

main: main.c $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) -o main main.c $(OBJECTS)

tests: tests.c $(OBJECTS) $(HEADERS)
	gcc $(CFLAGS) -o tests tests.c $(OBJECTS)

clean: 
	rm -f main tests $(OBJECTS)