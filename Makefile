CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11

TARGET = build/server

SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET) 
