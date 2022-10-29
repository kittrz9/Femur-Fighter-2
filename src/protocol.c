#include "protocol.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "netGameState.h"

const char* clientMethodStrings[] = {
	"CONNECT",
	"GETSTATE",
	"TEST",
	"DISCONNECT",
};

const char* serverMethodStrings[] = {
	"GETSTATE",
	"DISCONNECTED",
	"TEST",
	"HUH?",
};

// these are all the same but probably wont be in the future
const char* clientResposneStrings[] = {
	"TEST",
	"DISCONNECT",
	"NOTIMPLEMENTED",
	"OK",
};

const char* serverResponseStrings[] = {
	"TEST",
	"DISCONNECT",
	"NOTIMPLEMENTED",
	"OK",
};

const char noArgs[] = "NONE";

void printBuffer(void* buffer, size_t size) {
	for(size_t i = 0; i < size; ++i) {
		if(((char*)buffer)[i] > ' ' && ((char*)buffer)[i] < 0xFF) {
			printf("%c", ((char*)buffer)[i]);
		} else {
			printf("\\x%02x", ((char*)buffer)[i]);
		}
	}
	printf("\n");

	return;
}

PR_SERVER_RESPONSE clientRequest(connection c, PR_CLIENT_METHOD method, void* args, size_t argsLen){
	char request[MAX_REQUEST_LEN];
	size_t requestLen = 0;

	packet requestPacket;
	memcpy(requestPacket.header, clientMethodStrings[method], strlen(clientMethodStrings[method]));
	requestPacket.headerLen = strlen(clientMethodStrings[method]);
	if(args == NULL) {
		args = (char*)noArgs;
		argsLen = strlen(noArgs);
	}
	memcpy(requestPacket.args, args, argsLen);
	requestPacket.argsLen = argsLen;

	generatePacket(requestPacket, request, &requestLen);

	printf("--SENT REQUEST--\n");
	printBuffer(request, requestLen+1);

	if(send(c.socket, request, requestLen, 0) != requestLen) {
		fprintf(stderr, "could not send request \"%s\": %i\n", request, errno);
	} else {
		printf("request successfully sent\n");
	}

	char response[MAX_RESPONSE_LEN];

	size_t responseLen = recv(c.socket, response, MAX_RESPONSE_LEN, 0);
	if(responseLen < 0) {
		fprintf(stderr, "failed to receive response: %i\n", errno);
	}
	
	printf("--RECEIVED RESPONSE--\n");
	printBuffer(response, responseLen);

	return PR_RSP_SV_TEST;
}
PR_CLIENT_RESPONSE serverRequest(connection c, PR_SERVER_METHOD method, void* args, size_t argsLen){
	return PR_RSP_CL_TEST;
}

PR_SERVER_METHOD clientResponse(connection c){
	return PR_RSP_SV_TEST;
}
PR_CLIENT_METHOD serverResponse(connection c){
	char request[MAX_REQUEST_LEN]; //recieved not sent
	size_t requestSize = recv(c.socket, request, MAX_REQUEST_LEN, 0);

	printf("--RECEIVED REQUEST--\n");
	printBuffer(request, requestSize+1);

	PR_CLIENT_METHOD clientMethodCode = PR_MTH_CL_TEST;
	char response[MAX_RESPONSE_LEN];
	size_t responseSize = 0;
	packet requestPacket = parsePacket(request, requestSize);
	for(size_t i = 0; i < sizeof(clientMethodStrings)/sizeof(char*); ++i) {
		if(strncmp(requestPacket.header, clientMethodStrings[i], requestPacket.headerLen) == 0) {
			clientMethodCode = (PR_CLIENT_METHOD)i;
		}
	}

	packet responsePacket;
	switch(clientMethodCode) {
		case PR_MTH_CL_TEST:
			strncpy(responsePacket.header, requestPacket.header, requestPacket.headerLen);
			strncpy(responsePacket.args, requestPacket.args, requestPacket.argsLen);
			responsePacket.headerLen = requestPacket.headerLen;
			responsePacket.argsLen = requestPacket.argsLen;
			break;
		case PR_MTH_CL_DISCONNECT:
			memcpy(responsePacket.header, serverResponseStrings[PR_RSP_SV_DISCONNECT], strlen(serverResponseStrings[PR_RSP_SV_DISCONNECT]));
			memcpy(responsePacket.args, noArgs, strlen(noArgs));
			responsePacket.headerLen = strlen(serverResponseStrings[PR_RSP_SV_DISCONNECT]);
			responsePacket.argsLen = strlen(noArgs);
			break;
		case PR_MTH_CL_GETSTATE:
			serializedState s = serializeGameState(netGameState);
			printBuffer(s.buffer, s.size);
			memcpy(responsePacket.header, serverResponseStrings[PR_RSP_SV_OK], strlen(serverResponseStrings[PR_RSP_SV_OK]));
			memcpy(responsePacket.args, s.buffer, s.size);
			responsePacket.headerLen = strlen(serverResponseStrings[PR_RSP_SV_OK]);
			responsePacket.argsLen = s.size;
			break;
		default: // probably stupid to be repeating this code but whatever
			memcpy(responsePacket.header, serverResponseStrings[PR_RSP_SV_NOTIMPLEMENTED], strlen(serverResponseStrings[PR_RSP_SV_NOTIMPLEMENTED]));
			memcpy(responsePacket.args, noArgs, strlen(noArgs));
			responsePacket.headerLen = strlen(serverResponseStrings[PR_RSP_SV_NOTIMPLEMENTED]);
			responsePacket.argsLen = strlen(noArgs);
			break;
	}

	generatePacket(responsePacket, (char*)response, &responseSize);
	printBuffer(response, responseSize);

	if(send(c.socket, response, responseSize+1, 0) != responseSize+1) {
		fprintf(stderr, "could not send response \"%s\": %i\n", response, errno);
	}
	printf("--SENT RESPONSE--\n");
	printBuffer(response, responseSize+1);

	return clientMethodCode;
}

packet parsePacket(char* packetStr, size_t packetLen) {
	packet p;
	p.headerLen = 0;
	p.argsLen = 0;
	size_t i;

	for(i = 0; i < packetLen; ++i) {
		if(packetStr[i] == '/') { p.headerLen = i; break; }
		p.header[i] = packetStr[i];
	}
	if(p.headerLen == 0) {
		fprintf(stderr, "failed to parse header\n");
	}
	for(;i < packetLen; ++i) {
		if(packetStr[i] == '/') { p.argsLen = i; break; }
		p.args[i] = packetStr[i];
	}
	if(p.argsLen == 0) {
		fprintf(stderr, "failed to parse header\n");
	}



	return p;
}

void generatePacket(packet p, char* str, size_t* size) {
	printf("%p\n", str);
	memcpy(str, p.header, p.headerLen);
	str[p.headerLen] = '/';
	memcpy(str + p.headerLen + 1, p.args, p.argsLen);
	*size = p.headerLen + p.argsLen + 1;
	str[*size] = '/';
	str[*size + 1] = '\0';
	printf("%s\n", str);


	return;
}
