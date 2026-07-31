PROJECT := 05-embedded-test-debug
BUILD_DIR := build
CC ?= gcc

CPPFLAGS := \
	-Iinclude \
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
	-MMD \
	-MP

SANITIZER_FLAGS := \
	-fsanitize=address,undefined \
	-fno-omit-frame-pointer

CORE_SOURCES := $(filter-out src/main.c,$(wildcard src/*.c))
DEMO_SOURCES := $(CORE_SOURCES) src/main.c
TEST_SOURCES := $(CORE_SOURCES) $(wildcard tests/*.c)

DEMO := $(BUILD_DIR)/embedded_test_debug_demo
TEST_RUNNER := $(BUILD_DIR)/test_runner
TEST_SANITIZE := $(BUILD_DIR)/test_runner_sanitize

.PHONY: all demo run test sanitize list-labs validate clean help

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
	  '01-unit-test-event-queue' \
	  '02-unit-test-event-pool' \
	  '03-unit-test-state-machine' \
	  '04-event-spy' \
	  '05-fake-timer' \
	  '06-uart-shell' \
	  '07-binary-event-trace' \
	  '08-queue-latency' \
	  '09-handler-execution-time' \
	  '10-crash-record' \
	  '11-hardfault-record' \
	  '12-automated-use-case' \
	  '13-fault-injection' \
	  '14-regression-test' \
	  '15-stress-test' \
	  '16-soak-test' \
	  '17-ci' \
	  '18-hardware-in-the-loop'
validate:
	./tools/validate.sh

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo 'make            Build demo and run root tests'
	@echo 'make run        Run integrated Test/Debug demo'
	@echo 'make test       Run root host tests'
	@echo 'make sanitize   Run root tests with ASan/UBSan'
	@echo 'make list-labs  List 15 independent labs'
	@echo 'make validate   Run root and all lab validation'
	@echo 'make clean      Remove root build output'
