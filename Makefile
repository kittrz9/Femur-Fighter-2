CC = gcc
SHELL = /bin/bash
LIBS = -lSDL2 -lSDLmain -lSDL2_ttf -lSDL2_image
INCLUDE = -Isrc/ -Itpl/src/
CFLAGS = -Wall -Wno-missing-braces -O2
NAME = FemurFighter2
CLIENT_SOURCES = ${wildcard src/client/gameStates/*.c} ${wildcard src/client/*.c}
CLIENT_OBJS = $(subst src/,obj/,$(subst .c,.o,${CLIENT_SOURCES}))
SERVER_SOURCES = ${wildcard src/server/gameStates/*.c} ${wildcard src/server/*.c}
SERVER_OBJS = $(subst src/,obj/,$(subst .c,.o,${SERVER_SOURCES}))
COMMON_SOURCES = ${wildcard src/*.c}
COMMON_OBJS = $(subst src/,obj/,$(subst .c,.o,${COMMON_SOURCES}))
TPL_SOURCES = tpl/src/tpl.c
TPL_OBJS = obj/tpl.o

client: build-dir obj-dir $(CLIENT_OBJS) $(COMMON_OBJS) $(TPL_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) -Isrc/client/ -o build/${NAME}-client $(CLIENT_OBJS) $(COMMON_OBJS) $(TPL_OBJS)

server: build-dir obj-dir $(SERVER_OBJS) $(COMMON_OBJS) $(TPL_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) -Isrc/server/ -o build/${NAME}-server $(SERVER_OBJS) $(COMMON_OBJS) $(TPL_OBJS)

all: client server

build-dir:
	-mkdir -p build/

obj-dir:
	-mkdir -p obj/client/gameStates obj/server/gameStates

clean:
	-rm -rf obj/
	-rm build/${NAME}*

#$(OBJS): obj/%.o : src/%.c
#	$(CC) $(CFLAGS) $(INCLUDE) -g -c "src/$*.c" -o obj/$*.o


$(CLIENT_OBJS): obj/client/%.o : src/client/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -g -c "src/client/$*.c" -o obj/client/$*.o

$(SERVER_OBJS): obj/server/%.o : src/server/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -g -c "src/server/$*.c" -o obj/server/$*.o

$(COMMON_OBJS): obj/%.o : src/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -g -c "src/$*.c" -o obj/$*.o

$(TPL_OBJS): obj/%.o : tpl/src/%.c
	echo $*
	# no cflags because I don't want to get the warnings from it
	$(CC) $(INCLUDE) $(LIBS) -g -pg -c "tpl/src/$*.c" -o obj/$*.o
