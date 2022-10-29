#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "connection.h"
#include "listener.h"

#include "netGameState.h"
#include "tpl.h"
#include "protocol.h"

const netPlayer template = {
	.pos = {
		.x = 0.0f,
		.y = 0.0f,
	},
	.vel = {
		.x = 0.0f,
		.y = 0.0f,
	},
	.size = {
		.x = 150.0f,
		.y = 200.0f,
	},
	.dashTimer = 0,
	.dashCooldown = 0,
	.jumpCounter = 1,
	.facingLeft = 0,
};

int main(int argc, char** argv) {
	listener l = listenOnPort(42069);

	gameStateStruct state;
	tpl_node* tn;
	tn = tpl_map("S(f#c#)#", &state, 9, 3, 2);

	while(true) {
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof(their_addr);
		connection c;
		c.socket = accept(l, (struct sockaddr*)&their_addr, &addr_size);
		printf("connected\n");

		bool closing = false;

		while(!closing) {
			/*#define MAX_REQUEST_LEN 4096
			char buffer[MAX_REQUEST_LEN];
			size_t size = recv(c.socket, buffer, MAX_REQUEST_LEN, 0);
			buffer[size] = '\0';

			printf("%s\n", buffer);

			#define MAX_RESPONSE_LEN 4096
			void** response;
			size = MAX_RESPONSE_LEN;
			if(strncmp(buffer, "CLOSING", strlen("CLOSING")) == 0) {
				strcpy((char*)response, "CLOSING\n");
				printf("!!!\n");
				closing = true;
			} else {
				memcpy(&state.players[0], &template, sizeof(netPlayer));
				tpl_pack(tn, 0);

				tpl_dump(tn, TPL_MEM, &response, &size);
				if(send(c.socket, response, size, 0) != size) {
					printf("send failed: %i\n", errno);
				}
			}*/

			PR_CLIENT_METHOD response = serverResponse(c);
			//if(response == PR_RSP_CL_DISCONNECT || response == PR_RSP_CL_NOTIMPLEMENTED) {
			if(response == PR_MTH_CL_DISCONNECT) {
				printf("!!!\n");
				closing = true;
			}


			if(closing) {
				close(c.socket);
			}
		}
	}

	stopListening(l);

	return 0;
}
