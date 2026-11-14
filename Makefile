CC      := gcc
CFLAGS  := -Wall -Wextra -std=c11 -O2 -Isrc
LDLIBS  := -lm

SRC      := $(shell find src -name '*.c')
OBJ      := $(SRC:src/%.c=build/obj/%.o)
TESTS    := $(wildcard tests/test_*.c)
TEST_BIN := $(TESTS:tests/%.c=build/tests/%)

.PHONY: all test clean

all: $(OBJ)

# src/Foo/foo.c -> build/obj/Foo/foo.o (-MMD : suit les dépendances aux .h)
build/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# tests/test_foo.c + tous les objets -> build/tests/test_foo
build/tests/%: tests/%.c $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(OBJ) -o $@ $(LDLIBS)

# lance tous les tests
test: $(TEST_BIN)
	@for t in $(TEST_BIN); do echo "== $$t"; ./$$t || exit 1; done

# lance un seul test : make run-contrast_transformation
run-%: build/tests/test_%
	./$<

clean:
	rm -rf build output

-include $(OBJ:.o=.d)