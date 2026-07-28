TARGET := lab04-static-memory-budget
BUILD_DIR ?= build
CC ?= gcc
CFLAGS := -std=c11 -O0 -g3 -Wall -Wextra -Werror -Wshadow \
	-Wconversion -Wsign-conversion -pedantic \
	-fsanitize=address,undefined -fno-omit-frame-pointer -Iinclude
LDFLAGS := -fsanitize=address,undefined
SOURCES := src/main.c
TEST_SOURCES := 
DEMO := $(BUILD_DIR)/$(TARGET)
TEST := $(BUILD_DIR)/$(TARGET)-tests

.PHONY: all test run clean

all: $(DEMO) test
$(DEMO): $(SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@

test:
	@echo "No separate unit-test binary for this lab."

run: all
	ASAN_OPTIONS=detect_leaks=1 $(DEMO)
	
clean:
	rm -rf $(BUILD_DIR)
