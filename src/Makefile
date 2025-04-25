
TARGET = yokon
SRC_FILES = main.c game.c card.c utils.c io.c

CC = gcc
CFLAGS = -Wall -g

all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILES)

run: $(TARGET)
	./$(TARGET)

clean: 
	rm -f $(TARGET)
