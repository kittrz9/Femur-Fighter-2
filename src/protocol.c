#include "protocol.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_REQUEST_LEN 4096
#define MAX_RESPONSE_LEN 4096

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
};

const char* serverResponseStrings[] = {
	"TEST",
	"DISCONNECT",
	"NOTIMPLEMENTED",
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

	size_t methodLen = strlen(clientMethodStrings[method]);

	strncpy(request, clientMethodStrings[method], methodLen);
	request[methodLen] = '/';
	if(args == NULL) {
		args = (char*)noArgs;
		argsLen = strlen(noArgs);
	}
	memcpy(request + methodLen + 1, args, argsLen);

	size_t requestLen = methodLen + argsLen + 1;
	request[requestLen] = '/';

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
	printBuffer(response, responseLen+1);

	return PR_RSP_SV_TEST;
}
PR_CLIENT_RESPONSE serverRequest(connection c, PR_SERVER_METHOD method, void* args, size_t argsLen){
}

PR_SERVER_RESPONSE clientResponse(connection c){
	
}
PR_CLIENT_RESPONSE serverResponse(connection c){
	PR_CLIENT_RESPONSE responseCode = PR_RSP_CL_NOTIMPLEMENTED;
	char request[MAX_REQUEST_LEN]; //recieved not sent
	size_t requestSize = recv(c.socket, request, MAX_REQUEST_LEN, 0);

	printf("--RECEIVED REQUEST--\n");
	printBuffer(request, requestSize+1);

	PR_CLIENT_METHOD clientMethodCode;
	char response[MAX_RESPONSE_LEN];
	size_t responseSize;
	char requestHead[32]; // arbitrary size
	size_t requestHeadLen = 0; // maybe dumb to have both of these idk
	char responseHead[32];
	size_t responseHeadLen = 0;
	char responseArgs[MAX_RESPONSE_LEN-32];
	size_t responseArgsLen = 0;
	for(size_t i = 0; i < requestSize; ++i) {
		if(request[i] == '/') { requestHeadLen = i; break; }
		requestHead[i] = request[i];
	}
	for(size_t i = 0; i < sizeof(clientMethodStrings)/sizeof(char*); ++i) {
		if(strncmp(requestHead, clientMethodStrings[i], requestHeadLen) == 0) {
			clientMethodCode = (PR_CLIENT_METHOD)i;
		}
	}

	switch(clientMethodCode) {
		case PR_MTH_CL_TEST:
			memcpy(responseHead, requestHead, requestHeadLen);
			responseHeadLen = requestHeadLen;
			memcpy(responseArgs, request + requestHeadLen + 1, requestSize - requestHeadLen - 1);
			responseArgsLen = requestSize - requestHeadLen - 1;
			responseSize = requestSize;
			responseCode = PR_RSP_SV_TEST;
			break;
		case PR_MTH_CL_DISCONNECT:
			responseHeadLen = strlen(serverResponseStrings[PR_RSP_SV_DISCONNECT]);
			memcpy(responseHead, serverResponseStrings[PR_RSP_SV_DISCONNECT], responseHeadLen);
			responseArgsLen = strlen(noArgs);
			memcpy(responseArgs, noArgs, responseArgsLen);
			break;
		default: // probably stupid to be repeating this code but whatever
			responseHeadLen = strlen(serverResponseStrings[PR_RSP_SV_NOTIMPLEMENTED]);
			memcpy(responseHead, serverResponseStrings[PR_RSP_SV_NOTIMPLEMENTED], responseHeadLen);
			responseArgsLen = strlen(noArgs);
			memcpy(responseArgs, noArgs, responseArgsLen);
			break;
	}

	printBuffer(responseHead, responseHeadLen);
	printBuffer(responseArgs, responseArgsLen);
	memcpy(response, responseHead, responseHeadLen);
	response[responseHeadLen] = '/';
	memcpy(response + responseHeadLen + 1, responseArgs, responseArgsLen);
	response[responseSize] = '/';
	printBuffer(response, responseSize);
	response[responseSize] = '/';

	if(send(c.socket, response, responseSize, 0) != responseSize) {
		fprintf(stderr, "could not send response \"%s\": %i\n", response, errno);
	}
	printf("--SENT RESPONSE--\n");
	printBuffer(response, responseSize+1);

	return responseCode;
}
