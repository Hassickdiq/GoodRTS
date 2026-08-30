/*
 * GoodRTS
 * Copyright (C) Copyright (C) 2026 Batuhan Demir (Hassickdiq)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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
