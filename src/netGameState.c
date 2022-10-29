#include "netGameState.h"

#include "tpl.h"

gameStateStruct netGameState;

serializedState serializeGameState(gameStateStruct s) {
	tpl_node* tn;
	tn = tpl_map("S(f#c#)#", &s, 9, 3, 2);

	void* buffer;
	size_t size;

	tpl_pack(tn, 0);
	tpl_dump(tn, TPL_MEM, &buffer, &size);
	serializedState serialized;
	serialized.buffer = buffer;
	serialized.size = size;
	return serialized;
}
