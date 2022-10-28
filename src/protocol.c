#include "protocol.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_REQUEST_LEN 4096
#define MAX_RESPONSE_LEN 4096

const char* clientMethodStrings[] = {
	"CONNECT",
	"GETSTATE",
	"DISCONNECT",
	"TEST",
};

const char* serverMethodStrings[] = {
	"GETSTATE",
	"DISCONNECTED",
	"TEST",
	"HUH?",
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
	printBuffer(request, requestLen);

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
}

PR_SERVER_RESPONSE clientResponse(connection c){
	
}
PR_CLIENT_RESPONSE serverResponse(connection c){
	PR_CLIENT_RESPONSE responseCode;
	char request[MAX_REQUEST_LEN]; //recieved not sent
	size_t requestSize = recv(c.socket, request, MAX_REQUEST_LEN, 0);
	request[requestSize] = '\0';

	printf("--RECEIVED REQUEST--\n");
	printBuffer(request, requestSize);

	char response[MAX_RESPONSE_LEN];
	size_t responseSize;
	if(strncmp(request, clientMethodStrings[PR_MTH_CL_TEST], strlen(clientMethodStrings[PR_MTH_CL_TEST])) == 0){
		responseSize = requestSize;
		memcpy(response, request, requestSize);
		responseCode = PR_RSP_CL_TEST;
	} else if(strncmp(request, clientMethodStrings[PR_MTH_CL_DISCONNECT], strlen(clientMethodStrings[PR_MTH_CL_DISCONNECT])) == 0){
		responseSize = strlen("DISCONNECT//");
		memcpy(response, "DISCONNECT//", strlen("DISCONNECT//"));
		responseCode = PR_RSP_CL_DISCONNECT;
	} else {
		responseSize = strlen("NOTIMPLEMENTED//");
		memcpy(request, "NOTIMPLEMENTED//", strlen("NOTIMPLEMENTED//"));
		responseCode = PR_RSP_CL_NOTIMPLEMENTED;
	}

	if(send(c.socket, response, responseSize, 0) != responseSize) {
		fprintf(stderr, "could not send response \"%s\": %i\n", response, errno);
	}
	printf("--SENT RESPONSE--\n");
	printBuffer(response, responseSize);

	return responseCode;
}
