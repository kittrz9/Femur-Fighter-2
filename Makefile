CC = gcc
SHELL = /bin/bash
LIBS = -lSDL2 -lSDLmain -lSDL2_ttf
CFLAGS = -Wall -O2
NAME = SDL-thing
SOURCES = ${wildcard src/gameStates/*.c} ${wildcard src/*.c}

${NAME}: ${SOURCES}
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $@ $(LIBS)

debug: ${SOURCES}
	$(CC) $(CFLAGS) -g $(SOURCES) $(LDFLAGS) -o ${NAME}-debug $(LIBS)

all: ${NAME} debug

clean:
	rm ./${NAME}*
	# might be a bad idea here because if there's anything with the same name as the name will be deleted but like why would you have something named like "SDL-thing.c" or anything that's important named that here
