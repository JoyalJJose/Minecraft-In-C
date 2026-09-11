CC = gcc
GLFW_DIR = lib/glfw-3.5.1

CFLAGS = -Wall -Wextra -I$(GLFW_DIR)/include -I$(GLFW_DIR)/deps -Isrc
LDFLAGS = -L$(GLFW_DIR)/build/src
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lm

TARGET = build/minecraft
SRC = $(shell find src -name '*.c')
OBJ = $(SRC:src/%.c=build/%.o)

.PHONY: run clean

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@LD_LIBRARY_PATH="/usr/lib/wsl/lib$${LD_LIBRARY_PATH:+:$$LD_LIBRARY_PATH}" \
	GALLIUM_DRIVER=d3d12 \
	MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA \
	./$(TARGET)

clean:
	rm -rf build/*
