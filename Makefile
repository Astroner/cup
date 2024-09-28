CC=gcc-14
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=run

.PHONY: all
all: $(EXECUTABLE)
	./$(EXECUTABLE) $(EFLAGS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $(EXECUTABLE) $(CFLAGS) $^

$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -Wall -Wextra -std=c99 -pedantic -c -o $@ $<

.PHONY: tests
tests: lib
	$(CC) -o exec-tests -Wall -Wextra -std=c99 -pedantic -Isrc tests/main.spec.c
	./exec-tests
	rm ./exec-tests

lib: src/cup.c src/cup.h src/internals.h
	cat src/cup.h > Cup.h
	echo "#if defined(CUP_IMPLEMENTATION)" >> Cup.h
	tail -n +2 src/internals.h >> Cup.h
	tail -n +2 src/helpers.c >> Cup.h
	tail -n +2 src/cup.c >> Cup.h
	echo "#endif // CUP_IMPLEMENTATION" >> Cup.h

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECTS) test
