CC = gcc
SHELL = /bin/bash
LIBS = -lSDL2 -lSDLmain -lSDL2_ttf -lSDL2_image
CFLAGS = -Wall -Wno-missing-braces -O2
NAME = FemurFighter2
SOURCES = ${wildcard src/gameStates/*.c} ${wildcard src/*.c}
OBJS = $(subst src/,obj/,$(subst .c,.o,${SOURCES}))

${NAME}: build-dir obj-dir $(SOURCES) $(OBJS) 
	$(CC) $(CFLAGS) $(LIBS) -o build/$@ $(OBJS)

debug: ${SOURCES}
	$(CC) $(CFLAGS) -g $(SOURCES) $(LDFLAGS) -o build/${NAME}-debug $(LIBS)

all: ${NAME} debug

build-dir:
	-mkdir -p build/

obj-dir:
	-mkdir -p obj/gameStates

clean:
	-rm -rf obj/
	-rm build/${NAME}*

$(OBJS): obj/%.o : src/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -g -c "src/$*.c" -o obj/$*.o
