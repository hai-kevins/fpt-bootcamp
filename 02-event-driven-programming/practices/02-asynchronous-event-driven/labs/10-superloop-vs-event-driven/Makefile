BUILD_DIR ?= build
CC ?= gcc
CFLAGS := -std=c11 -O0 -g3 \
	-Wall -Wextra -Werror -Wshadow -Wundef \
	-Wconversion -Wsign-conversion \
	-fsanitize=address,undefined
SUPERLOOP := $(BUILD_DIR)/superloop_demo
EVENT_DRIVEN := $(BUILD_DIR)/event_driven_demo

.PHONY: all run clean rebuild

all: $(SUPERLOOP) $(EVENT_DRIVEN)

$(SUPERLOOP): superloop.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@
	
$(EVENT_DRIVEN): event_driven.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

run: all
	@echo "=== Super-loop ==="
	$(SUPERLOOP)
	@echo
	@echo "=== Event-Driven ==="
	$(EVENT_DRIVEN)

rebuild: clean all

clean:
	rm -rf $(BUILD_DIR)
