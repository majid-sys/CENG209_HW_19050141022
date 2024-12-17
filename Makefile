# Makefile for Adventure Game

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = adventure_game

all: $(TARGET)

$(TARGET): adventure_game.c
	$(CC) $(CFLAGS) -o $(TARGET) adventure_game.c

clean:
	rm -f $(TARGET)
