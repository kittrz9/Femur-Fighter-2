#include "listener.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

listener listenOnPort(uint16_t port) {
	listener l;
	struct addrinfo hints;
	struct addrinfo* res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; // TCP and not UDP just for testing
	hints.ai_flags |= AI_PASSIVE;

	char portStr[6];
	sprintf(portStr, "%i", port);

	int status = getaddrinfo(NULL, portStr, &hints, &res);
	if(status != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
		exit(1);
	}

	l = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(l < 0) {
		fprintf(stderr, "could not create socket\n");
		exit(1);
	}
	freeaddrinfo(res);

	int yes=1;
	if(setsockopt(l, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	if(bind(l, res->ai_addr, res->ai_addrlen) != 0) {
		fprintf(stderr, "could not bind port\n");
		exit(1);
	}

	if(listen(l, 10) != 0) {
		perror("listen");
		exit(1);
	}

	printf("listening on port %i\n", port);

	return l;
}

void stopListening(listener l) {
	printf("closing socket %i\n", (int)l);
	close(l); // probably dumb to have a function just to call the close function but whatever
	return;
}
