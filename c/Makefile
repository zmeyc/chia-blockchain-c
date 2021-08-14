SRC = $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))
DEP = $(OBJ:%.o=%.d)

TESTS_SRC = $(wildcard tests/*.c) $(wildcard tests/*/*.c) $(wildcard tests/*/*/*.c)
TESTS_OBJ = $(addprefix obj_tests/,$(notdir $(TESTS_SRC:.c=.o)))
TESTS_DEP = $(TESTS_OBJ:%.o=%.d)

CC=gcc
AR=ar

CFLAGS = -std=c99 -pedantic -g -O2 -MMD -Wall -Werror
LFLAGS = -shared

TESTS_CFLAGS = -std=c99 -pedantic -g -O2 -MMD -Wall -Werror -Itests -Isrc
TESTS_LFLAGS = -Lobj -lchia

all: obj/libchia.so obj/libchia.a obj_tests/chia-test

test: obj_tests/chia-test
	obj_tests/chia-test

obj/libchia.so: $(OBJ)
	$(CC) $(OBJ) $(LFLAGS) -o $@

obj/libchia.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

obj_tests/chia-test: $(TESTS_OBJ) obj/libchia.a obj/libchia.so
	$(CC) $(TESTS_OBJ) $(TESTS_LFLAGS) -o $@

obj:
	mkdir -p obj

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/*/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/*/*/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj_tests:
	mkdir -p obj_tests

obj_tests/%.o: tests/%.c | obj_tests
	$(CC) $< -c $(TESTS_CFLAGS) -o $@

obj_tests/%.o: tests/*/%.c | obj_tests
	$(CC) $< -c $(TESTS_CFLAGS) -o $@

obj_tests/%.o: tests/*/*/%.c | obj_tests
	$(CC) $< -c $(TESTS_CFLAGS) -o $@

clean:
	rm -rf obj obj_tests

-include $(DEP)
-include $(TESTS_DEP)

.PHONY: all test clean

