CC = gcc

GLFW_DIR = lib/glfw-3.5.1

CFLAGS = -Wall -Wextra -I$(GLFW_DIR)/include
LDFLAGS = -L$(GLFW_DIR)/build/src
LDLIBS = -lglfw3 -lGL -lX11 -lpthread -lm

TARGET = minecraft

SRC = src/main.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
