CC = gcc
GLFW_DIR = lib/glfw-3.5.1

CFLAGS = -Wall -Wextra -O2 -I$(GLFW_DIR)/include -I$(GLFW_DIR)/deps -Isrc
LDFLAGS = -L$(GLFW_DIR)/build/src
LDLIBS = -lglfw3 -lopengl32 -lgdi32 -luser32 -lshell32

TARGET = build/minecraft
SRC = $(shell find src -name '*.c')
OBJ = $(SRC:src/%.c=build/%.o)

.PHONY: all run clean glfw

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@./$(TARGET)

clean:
	@rm -rf build/*
