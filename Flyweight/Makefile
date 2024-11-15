CC = g++
CFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lSDL2 -lSDL2_image

SRC = src/main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = flyweight_sdl

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)



