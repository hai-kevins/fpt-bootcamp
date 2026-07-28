CC ?= cc
BUILD_DIR := build
CPPFLAGS := -Iinclude
CFLAGS := -std=c11 -Og -g3 -Wall -Wextra -Werror -Wshadow -Wpedantic \
          -fsanitize=address,undefined -fno-omit-frame-pointer
LDFLAGS := -fsanitize=address,undefined
LIB_SOURCES := $(filter-out src/main.c,$(wildcard src/*.c))
TEST_SOURCES := $(wildcard tests/*.c)
DEMO_SOURCES := $(LIB_SOURCES) src/main.c
TEST_BIN := $(BUILD_DIR)/tests
DEMO_BIN := $(BUILD_DIR)/demo

.PHONY: all test run clean help

all: test
$(TEST_BIN): $(LIB_SOURCES) $(TEST_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@
$(DEMO_BIN): $(DEMO_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDFLAGS) -o $@

test: $(TEST_BIN)
	ASAN_OPTIONS=detect_leaks=1 ./$(TEST_BIN)

run: $(DEMO_BIN)
	./$(DEMO_BIN)

clean:
	rm -rf $(BUILD_DIR)
	
help:
	@echo "make test  Build and run local tests"
	@echo "make run   Build and run local demo"
	@echo "make clean Remove this lab build output"
