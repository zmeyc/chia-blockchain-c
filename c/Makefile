SRC = $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
SRC_CPP = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard src/*/*/*.cpp)
OBJ = $(addprefix obj/,$(notdir $(SRC:.c=.o)))
OBJ_CPP = $(addprefix obj/,$(notdir $(SRC_CPP:.cpp=.o))) obj/lzcnt.o
DEP = $(OBJ:%.o=%.d) $(OBJ_CPP:%.o=%.d)

TESTS_SRC = $(wildcard tests/*.c) $(wildcard tests/*/*.c) $(wildcard tests/*/*/*.c)
TESTS_OBJ = $(addprefix obj_tests/,$(notdir $(TESTS_SRC:.c=.o)))
TESTS_DEP = $(TESTS_OBJ:%.o=%.d)

CC=gcc
CXX=g++
AR=ar

CFLAGS_COMMON = -Ilibroot/include
LFLAGS_COMMON = -Llibroot/lib -lrelic_s -lgmp

CFLAGS = -std=c11 -pedantic -g -O2 -MMD -Wall -Werror $(CFLAGS_COMMON)
CFLAGS_CPP = -std=c++17 -pedantic -g -O2 -MMD -Wall -Werror -Wno-unused-variable
LFLAGS = -shared $(LFLAGS_COMMON)

TESTS_CFLAGS = -std=c11 -pedantic -g -O2 -MMD -Wall -Werror -Itests -Isrc $(CFLAGS_COMMON)
TESTS_LFLAGS = -Lobj -lchiaxx -lchia $(LFLAGS_COMMON)

all: obj/libchia.so obj/libchia.a obj/libchiaxx.so obj/libchiaxx.a obj_tests/chia-test

test: obj_tests/chia-test
	obj_tests/chia-test

obj/libchia.so: $(OBJ) obj/libchiaxx.so
	$(CC) $(OBJ) $(LFLAGS) -Lobj -lchiaxx -o $@

obj/libchia.a: $(OBJ)
	$(AR) rcs $@ $(OBJ)

obj/libchiaxx.so: $(OBJ_CPP)
	$(CXX) $(OBJ_CPP) $(LFLAGS) -o $@

obj/libchiaxx.a: $(OBJ_CPP)
	$(AR) rcs $@ $(OBJ_CPP)

obj_tests/chia-test: $(TESTS_OBJ) obj/libchia.a obj/libchia.so obj/libchiaxx.a obj/libchiaxx.so
	$(CC) $(TESTS_OBJ) $(TESTS_LFLAGS) -o $@

obj:
	mkdir -p obj

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/*/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/*/*/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj/%.o: src/%.cpp | obj
	$(CXX) $< -c $(CFLAGS_CPP) -o $@

obj/%.o: src/*/%.cpp | obj
	$(CXX) $< -c $(CFLAGS_CPP) -o $@

obj/%.o: src/*/*/%.cpp | obj
	$(CXX) $< -c $(CFLAGS_CPP) -o $@

obj/lzcnt.o: thirdparty/chiavdf/src/refcode/lzcnt.c | obj
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

