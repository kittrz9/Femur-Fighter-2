#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>
#include <netdb.h>

typedef struct {
	int socket;
	struct addrinfo* addr;
} connection;

// returns socket id
connection connectToServer(char* hostname, uint16_t port);
void disconnect(connection c);

#endif
