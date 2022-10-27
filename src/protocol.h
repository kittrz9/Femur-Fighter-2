#ifndef PROTOCOL_H
#define PROTOCOL_H

// idk how I'm going to make the whole protocol thing work yet, this is just a rough idea of how I might implement this
// I'll probably have functions here to do these things like serialize the game state and send that

/*
 * format of each packet is "(method)'\0'(tpl serialized args)"
 * also tick rate is probably just gonna be 60hz
 *
 * CLIENT->SERVER: "CONNECT":
 * 	ARGS: name(32 bytes), protocol version(probably like most recent git commit's id something idk)
 * 	RETURNS: "OK" if protocol versions match, returns "NO/(server protocol version)" if not
 * 	NOTES: only returns a response when both players have connected (probably should have it like this so it doesn't just have the screen freeze up while waiting for the other player, though I could probably have a separate thread for networking but that seems like it'd be really annoying to implement lmao)
 *
 * SERVER->CLIENT: "GETCLIENTSTATE":
 * 	ARGS: none
 * 	RETURNS: "STATE/", serialized state of the game (players position, health, etc.)
 * 	NOTES: sent to both players per tick,
 *
 * CLIENT->SERVER: "GETSERVERSTATE":
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
 */

#endif
