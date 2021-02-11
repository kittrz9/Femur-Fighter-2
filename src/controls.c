#include "controls.h" 

// I need this for some reason to have the extern thing work
key keys[CONTROLS_LENGTH]; 

void initControls() {
	// Really need to figure out how to do like a config file or something instead of hardcoding this stuff lmao
	keys[UP].keycode = SDLK_w;
	keys[DOWN].keycode = SDLK_s;
	keys[LEFT].keycode = SDLK_a;
	keys[RIGHT].keycode = SDLK_d;
    keys[DASH_LEFT].keycode = SDLK_q;
    keys[DASH_RIGHT].keycode = SDLK_e;
	keys[PAUSE].keycode = SDLK_ESCAPE;
    
    keys[UP2].keycode = SDLK_KP_8;
	keys[DOWN2].keycode = SDLK_KP_5;
	keys[LEFT2].keycode = SDLK_KP_4;
	keys[RIGHT2].keycode = SDLK_KP_6;
    keys[DASH_LEFT2].keycode = SDLK_KP_7;
    keys[DASH_RIGHT2].keycode = SDLK_KP_9;
    
    
    // Also I guess I need this because it isn't always just initialized to false(?)
	for(int i = 0; i < CONTROLS_LENGTH; i++){
		keys[i].held = false;
        keys[i].pressedTimer = 0.0;
	}
}
