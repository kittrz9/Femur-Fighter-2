#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdlib.h>

#include "connection.h"

// idk how I'm going to make the whole protocol thing work yet, this is just a rough idea of how I might implement this
// I'll probably have functions here to do these things like serialize the game state and send that

/*
 * format of each packet is "(method)/(tpl serialized args)/"
 * first / separates method and args, second / terminates the packet
 * also tick rate is probably just gonna be 60hz
 *
 * CLIENT->SERVER: "CONNECT":
 * 	ARGS: name(32 bytes), protocol version(probably like most recent git commit's id something idk)
 * 	RETURNS: "OK/(player number)" if protocol versions match, returns "NO/(server protocol version)" if not
 * 	NOTES: only returns a response when both players have connected (probably should have it like this so it doesn't just have the screen freeze up while waiting for the other player, though I could probably have a separate thread for networking but that seems like it'd be really annoying to implement lmao)
 *
 * SERVER->CLIENT: "GETSTATE":
 * 	ARGS: none
 * 	RETURNS: "STATE/", serialized state of the game (players position, health, etc.)
 * 	NOTES: sent to both players per tick,
 *
 * CLIENT->SERVER: "GETSTATE":
 * 	ARGS: none
 * 	RETURNS: "STATE/", serialized state of the game (players position, health, etc.)
 * 	NOTES: the "STATE/" at the start might not be necessary but could help with error checking
 *
 * SERVER->CLIENT: "HUH?":
 * 	ARGS: none
 * 	RETURNS: "STATE/", serialized state of the game (players position, health, etc.)
 * 	NOTES: sent when the clients aren't synced within a reasonable amount, will only check once before just reverting to player 1 I guess lmao
 * 	       I probably wont end up programming in cheat detection
 *	       question mark might be dumb but whatever I'll probably only use / for formatting, also it's funny
 *
 * CLIENT->SERVER: "DISCONNECT":
 * 	ARGS: none
 * 	RETURNS: "OK"
 * 	NOTES: sent when disconnecting so the other player can go back to the main menu and not stay stuck waiting for a response
 *
 * SERVER->CLIENT: "DISCONNECTED":
 * 	ARGS: none
 * 	RETURNS: "OK"
 * 	NOTES: see above
 *
 * CLIENT->SERVER: "TEST":
 * 	ARGS: any
 * 	RETURNS: args
 * 	NOTES: basically a ping command I think
 *
 */

// maybe having two enums for protocol methods is dumb but I feel like it could help with making sure you dont do client requests from the server
typedef enum {
	PR_MTH_CL_CONNECT,
	PR_MTH_CL_GETSTATE,
	PR_MTH_CL_DISCONNECT,
	PR_MTH_CL_TEST,
} PR_CLIENT_METHOD;

typedef enum {
	PR_MTH_SV_GETSTATE,
	PR_MTH_SV_DISCONNECTED,
	PR_MTH_SV_TEST,
} PR_SERVER_METHOD;

typedef enum {
	PR_RSP_SV_TEST,
	PR_RSP_SV_CLOSING,
	PR_RSP_SV_NOTIMPLEMENTED,
} PR_SERVER_RESPONSE;

typedef enum {
	PR_RSP_CL_TEST,
	PR_RSP_CL_DISCONNECT,
	PR_RSP_CL_NOTIMPLEMENTED,
} PR_CLIENT_RESPONSE;

PR_SERVER_RESPONSE clientRequest(connection c, PR_CLIENT_METHOD method, void* args, size_t argsLen);
PR_CLIENT_RESPONSE serverRequest(connection c, PR_SERVER_METHOD method, void* args, size_t argsLen);

PR_SERVER_RESPONSE clientResponse(connection c);
PR_CLIENT_RESPONSE serverResponse(connection c);

#endif
