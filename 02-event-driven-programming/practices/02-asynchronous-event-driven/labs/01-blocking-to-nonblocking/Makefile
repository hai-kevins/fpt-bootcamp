BUILD_DIR ?= build
CC ?= gcc
CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined

BLOCKING := $(BUILD_DIR)/blocking_demo
NONBLOCKING := $(BUILD_DIR)/nonblocking_demo

.PHONY: all run clean rebuild

all: $(BLOCKING) $(NONBLOCKING)

$(BLOCKING): blocking.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

$(NONBLOCKING): nonblocking.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

run: all
	@echo "=== Blocking ==="
	$(BLOCKING)
	@echo
	@echo "=== Non-blocking ==="
	$(NONBLOCKING)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)
