#ifndef LISTENER_H
#define LISTENER_H

#include <stdint.h>

typedef int listener; // it's just a socket but I think this will make it a bit more readable

listener listenOnPort(uint16_t port);
void stopListening(listener l);

#endif
