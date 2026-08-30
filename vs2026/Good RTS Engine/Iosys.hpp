#pragma once
#include "main.hpp"
#include "raylib.h"

struct GameIO {
	Vector2 MousePos = {0, 0};
	u16 lastKey = 0x00;
	u8 lastClick = 0x00;

	int16_t focus = -1;

	bool numpad[9];

	bool keyUp = false;
	bool keyDown = false;
	bool keyRight = false;
	bool keyLeft = false;
	bool ctrl = false;
	bool backspace = false;
	bool enter = false;
};