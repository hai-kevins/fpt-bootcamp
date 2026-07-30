PROJECT := 04-event-driven-system-components
BUILD_DIR := build
CC ?= gcc

CPPFLAGS := \
    -Iframework/include \
    -Idatalink/include \
    -Iapplication/include \
    -Itests

CFLAGS := \
    -std=c11 \
    -O0 \
    -g3 \
    -Wall \
    -Wextra \
    -Werror \
    -Wpedantic \
    -Wshadow \
    -Wconversion \
    -Wsign-conversion \
    -MMD \
    -MP

SANITIZER_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

FRAMEWORK_SOURCES := $(wildcard framework/src/*.c)
DATALINK_SOURCES := $(wildcard datalink/src/*.c)
APPLICATION_SOURCES := application/src/demo_ao.c
COMMON_SOURCES := $(FRAMEWORK_SOURCES) $(DATALINK_SOURCES) $(APPLICATION_SOURCES)

DEMO_SOURCES := $(COMMON_SOURCES) application/src/main.c
TEST_SOURCES := $(COMMON_SOURCES) tests/test_all.c

DEMO := $(BUILD_DIR)/event_driven_demo
TEST_RUNNER := $(BUILD_DIR)/test_runner
TEST_SANITIZE := $(BUILD_DIR)/test_runner_sanitize

.PHONY: all demo run test sanitize clean help list-labs

all: demo test

demo: $(DEMO)

$(DEMO): $(DEMO_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

run: $(DEMO)
	./$(DEMO)

$(TEST_RUNNER): $(TEST_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_SANITIZE): $(TEST_SOURCES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SANITIZER_FLAGS) $^ -o $@

sanitize: $(TEST_SANITIZE)
	./$(TEST_SANITIZE)

list-labs:
	@printf '%s\n' \
	  '01-active-object' \
	  '02-mailbox' \
	  '03-run-to-completion-scheduler' \
	  '04-flat-state-machine' \
	  '05-hierarchical-state-machine' \
	  '06-table-driven-state-machine' \
	  '07-event-pool' \
	  '08-reference-count-ownership' \
	  '09-router-direct-post' \
	  '10-publish-subscribe' \
	  '11-uart-datalink' \
	  '12-parser-state-machine' \
	  '13-link-failure' \
	  '14-event-trace' \
	  '15-stress-test'

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo 'make            Build demo and run tests'
	@echo 'make run        Run integrated host demo'
	@echo 'make test       Run 9 host tests'
	@echo 'make sanitize   Run tests with ASan/UBSan'
	@echo 'make list-labs  List included labs'
	@echo 'make clean      Remove root build output'
