CC = gcc
SHELL = /bin/bash
LIBS = -lSDL2 -lSDLmain -lSDL2_ttf -lSDL2_image
CFLAGS = -Wall -Wno-missing-braces -O2
NAME = FemurFighter2
SOURCES = ${wildcard src/gameStates/*.c} ${wildcard src/*.c}
OBJS = $(subst src/,obj/,$(subst .c,.o,${SOURCES}))

${NAME}: $(SOURCES) $(OBJS)
	# I feel like I'm doing this very wrong but whatever
	-rm $(subst src/,obj/,$(subst .c,.o,$?))
	make $(subst src/,obj/,$(subst .c,.o,$?))
	$(CC) $(CFLAGS) $(LIBS) -o $@ $(OBJS)

debug: ${SOURCES}
	$(CC) $(CFLAGS) -g $(SOURCES) $(LDFLAGS) -o ${NAME}-debug $(LIBS)

all: ${NAME} debug

clean:
	-rm obj/*.o obj/gameStates/*.o
	-rm ./${NAME}*
	# might be a bad idea here because if there's anything with the same name as the name will be deleted but like why would you have something named like "SDL-thing.c" or anything that's important named that here
	
%.o: 
	mkdir -p obj/gameStates
	$(CC) $(CFLAGS) $(LIBS) -c $(subst obj/,src/,$*.c) -o $(subst src/,obj/,$*.o)
