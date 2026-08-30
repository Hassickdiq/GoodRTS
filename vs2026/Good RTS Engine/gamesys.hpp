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
#include "worldsys.hpp"
#include "chacesys.hpp"
#include "guisys.hpp"
#include "Iosys.hpp"

class Game {
public:
	double globaltime = 0.0;
	GameWorld* world = nullptr;
	GameGUI* gui = nullptr;
	GameIO gios;
	lua_State* L = nullptr;

	Game();
	static Game* Quit(Game* game);
	GameWorld* LoadWorld(std::string path);
	void InitGameMenu(lua_State* L);
	static int LuaLoadMap(lua_State* L);

	void GameLoop();
};
