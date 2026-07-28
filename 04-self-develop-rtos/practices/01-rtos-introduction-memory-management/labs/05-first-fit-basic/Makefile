TARGET := lab05-first-fit-basic
BUILD_DIR ?= build
CC ?= gcc
CFLAGS := -std=c11 -O0 -g3 -Wall -Wextra -Werror -Wshadow \
	-Wconversion -Wsign-conversion -pedantic \
	-fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude
LDFLAGS := -fsanitize=address,undefined
SOURCES := src/heap.c src/main.c
TEST_SOURCES := tests/test_heap_init.c tests/test_main.c tests/test_heap_alloc.c
DEMO := $(BUILD_DIR)/$(TARGET)
TEST := $(BUILD_DIR)/$(TARGET)-tests

.PHONY: all test run clean

all: $(DEMO) test
$(DEMO): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

test:
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(filter-out src/main.c,$(SOURCES)) $(TEST_SOURCES) $(LDFLAGS) -o $(TEST)
	ASAN_OPTIONS=detect_leaks=1 $(TEST)

run: all
	ASAN_OPTIONS=detect_leaks=1 $(DEMO)
	
clean:
	rm -rf $(BUILD_DIR)
