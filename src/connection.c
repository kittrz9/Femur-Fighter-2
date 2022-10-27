#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

connection connectToServer(char* hostname, uint16_t port) {
	int socketID;
	struct addrinfo hints;
	struct addrinfo* res;

	if(port > 65535) {
		fprintf(stderr, "given port is too high, was given %u\n", port);
		exit(1);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;

	// has to be a string for getaddrinfo
	char portStr[6]; // max value is 65535
	sprintf(portStr, "%i", port);

	int status = getaddrinfo(hostname, portStr, &hints, &res);
	if(status != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
		exit(1);
	}

	// using the first ip address given by getaddrinfo even though it can give multiple
	// probably not a good thing idk, maybe I should change this
	void* addr;

	if(res->ai_family == AF_INET) {
		//ipv4
		struct sockaddr_in* ipv4 = (struct sockaddr_in*)res->ai_addr;
		addr = &(ipv4->sin_addr);
	} else {
		//ipv6
		struct sockaddr_in* ipv6 = (struct sockaddr_in6*)res->ai_addr;
		addr = &(ipv6->sin_addr);
	}
	char ipstr[INET6_ADDRSTRLEN];
	inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr));
	printf("connecting to: %s\n", ipstr);

	socketID = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	if(connect(socketID, res->ai_addr, res->ai_addrlen) < 0) {
		fprintf(stderr, "connect failed: %i\n", errno);
		exit(1);
	}

	printf("socket %i created\n", socketID);

	connection c = {
		.socket = socketID,
		.addr = res,
	};

	return c;
}

void disconnect(connection c) {
	if(close(c.socket) < 0) {
		fprintf(stderr, "could not close socket %i\n", c.socket);
	}

	freeaddrinfo(c.addr);
	return;
}
