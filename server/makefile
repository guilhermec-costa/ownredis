.PHONY: all default debug run clean

CC := gcc
OUTPUT_DIR := bin
EXEC := $(OUTPUT_DIR)/ownredis-server
ARGS = 

EXEC_DEBUG := $(OUTPUT_DIR)/ownredis-server-debug
CFLAGS := -Wall -Wextra -std=c99 -Iinclude
SRCS := $(shell find src -name "*.c")
OBJS := $(patsubst src/%.c, $(OUTPUT_DIR)/%.o, $(SRCS))

default: all

debug: CFLAGS += -g -O0
debug: clean all

all: $(EXEC)

run: all
	@chmod +x $(EXEC)
	$(EXEC) $(ARGS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUTPUT_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf bin