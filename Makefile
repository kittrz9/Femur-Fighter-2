CC = gcc
SHELL = /bin/bash
LIBS = -lSDL2 -lSDLmain -lSDL2_ttf -lSDL2_image
INCLUDE = -Isrc/
CFLAGS = -Wall -Wno-missing-braces -O2
NAME = FemurFighter2
CLIENT_SOURCES = ${wildcard src/client/gameStates/*.c} ${wildcard src/client/*.c}
CLIENT_OBJS = $(subst src/,obj/,$(subst .c,.o,${CLIENT_SOURCES}))
SERVER_SOURCES = ${wildcard src/server/gameStates/*.c} ${wildcard src/server/*.c}
SERVER_OBJS = $(subst src/,obj/,$(subst .c,.o,${SERVER_SOURCES}))
COMMON_SOURCES = ${wildcard src/*.c}
COMMON_OBJS = $(subst src/,obj/,$(subst .c,.o,${COMMON_SOURCES}))

#${NAME}: build-dir obj-dir $(SOURCES) $(OBJS) 
#	$(CC) $(CFLAGS) $(LIBS) -o build/$@ $(OBJS)

client: build-dir obj-dir $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) -Isrc/client/ -o build/${NAME}-client $(CLIENT_OBJS) $(COMMON_OBJS)

debug: ${SOURCES}
	$(CC) $(CFLAGS) -g $(SOURCES) $(LDFLAGS) -o build/${NAME}-debug $(LIBS)

all: ${NAME} debug

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

