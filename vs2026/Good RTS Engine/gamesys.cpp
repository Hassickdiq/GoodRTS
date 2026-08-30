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

#include "gamesys.hpp"
#include "mem.h"
#include <fstream>

using namespace std;

Game::Game() {
	SetTraceLogLevel(LOG_NONE);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Good RTS");

	if (!IsWindowReady()) {
		std::cout << "Could not be started Good RTS!" << std::endl;
		return;
	}

	ToggleFullscreen();
	SetTargetFPS(FPS_LIMIT);
	HideCursor();
	InitAudioDevice();

	L = luaL_newstate();
	if (!L) { cout << "Lua State başlatılamadı!\n"; return; }
	else{
		luaL_openlibs(L);

		lua_pushlightuserdata(L, this);
		lua_setfield(L, LUA_REGISTRYINDEX, "MyGameGameInstance");

		lua_pushcfunction(L, Game::LuaLoadMap);
		lua_setglobal(L, "LoadMap");
	}

	gui = new GameGUI(L);
}

Game* Game::Quit(Game* game) {
	if(game->world)
		game->world->Delete();
	if(game->gui)
		game->gui->Delete();

	game->gui   = nullptr;
	game->world = nullptr;
	delete game;
	return nullptr;
}

GameWorld* Game::LoadWorld(std::string path) {
	int len = path.length();

	std::string type = "NOP";
	type[2] = path[len - 1];
	type[1] = path[len - 2];
	type[0] = path[len - 3];

	for (char& c : type) {
		c = toupper(c);
	}


	if (type == "MAP") {
		std::ifstream file(path);
		if (!file.is_open()) {
			std::cerr << "File acilamadi!" << std::endl;
			return nullptr;
		}

		std::vector<std::string> buffer;
		std::string line;

		int len = 0;

		while (getline(file, line)) {
			buffer.push_back(line);
			len++;
		}

		int i = 0;

		GameWorld* newWorld = nullptr;
		vec2 size = { 10, 10 };
		u8 localPlr = 0;

		if (buffer[i] == "localPlr:") {
			i++;
			localPlr = stoi(buffer[i]) & 0xFF;
			i++;
		}

		if (buffer[i] == "size:") {
			i++;
			size.x = stoi(buffer[i]);
			i++;
			size.y = stoi(buffer[i]);
			i++;
		}

		newWorld = new GameWorld((u32)size.x, (u32)size.y, localPlr, L);
		cout << "New World " << size.x << "x" << size.y << " [" << newWorld << "]" << endl;

		if (buffer[i] == "*paths") {
			i++;

			while (buffer[i] != "*end") {
				newWorld->Cache.Load(buffer[i], newWorld->L);
				i++;
			}
			i++;
		}

		if (buffer[i] == "*tiles") {
			i++;
			ScxTile tile;
			u64 grid = 0;

			while (buffer[i] != "*end") {
				for (size_t j = 0; j < 3; j++) {
					if (j == 0) {
						tile.animation = newWorld->Cache.Animations[newWorld->Cache.TpTiles[buffer[i]].animPath];
						tile.offx = newWorld->Cache.TpTiles[buffer[i]].offX;
						tile.offy = newWorld->Cache.TpTiles[buffer[i]].offY;
					}
					else if (j == 1) // E
						tile.animation.index = stoi(buffer[i]);
					else if (j == 2) // E
						tile.elevation = stoi(buffer[i]);
					i++;
				}
				newWorld->map->Set(tile, (grid % size.x), (grid / size.x));
				grid++;
			}
			i++;
		}

		if (buffer[i] == "*entitys") {
			i++;
			string name;
			Vector2 origin = {0, 0};
			u8 player = 0;

			while (buffer[i] != "*end") {
				for (size_t j = 0; j < 4; j++) {
					if (j == 0) {
						name = buffer[i];
					}
					else if (j == 1) // P
						player = stoi(buffer[i]);
					else if (j == 2) // E
						origin.x = stof(buffer[i]);
					else if (j == 3) // E
						origin.y = stof(buffer[i]);
					i++;
				}
				newWorld->MakeEntity(&newWorld->Cache.TpEntitys[name], origin, player);
			}
		}

		return newWorld;
	}

	return nullptr;
}

void GetIO(GameIO* gios) {
	gios->ctrl = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
	gios->keyUp = IsKeyDown(KEY_UP);
	gios->keyDown = IsKeyDown(KEY_DOWN);
	gios->keyRight = IsKeyDown(KEY_RIGHT);
	gios->keyLeft = IsKeyDown(KEY_LEFT);
	gios->backspace = IsKeyPressedRepeat(KEY_BACKSPACE) || IsKeyPressed(KEY_BACKSPACE);
	gios->enter = IsKeyPressed(KEY_ENTER);

	u16 key = GetCharPressed();
	gios->lastKey = key;

	for (int i = 0; i < 9; i++) {
		gios->numpad[i] = IsKeyDown(48 + i);
	}

	gios->lastClick = 0;
	for (int i = 0; i < 3; i++) {
		if (IsMouseButtonPressed(i))
			gios->lastClick = i + 1;
	}

	gios->MousePos = GetMousePosition();
}

void Game::InitGameMenu(lua_State* L) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, "InitGameMenu");

	if (lua_isfunction(L, -1)) {
		if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

int Game::LuaLoadMap(lua_State* L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "MyGameGameInstance");
	Game* myGame = (Game*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	if (lua_isstring(L, 1)) {
		std::string path = lua_tostring(L, 1);
		myGame->world = myGame->LoadWorld(path);
	}

	return 0;
}

void Game::GameLoop() {
	gui->Cache.Load("data/ui/cursor.chc");
	Anim2D cursor = gui->Cache.Animations["cursor"];

	if (luaL_dofile(L, "scripts/gameMenu.lua") != LUA_OK) {
		std::cout << "Lua load error (scripts / gameMenu.lua): " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}

	InitGameMenu(L);

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		globaltime = GetTime();
		GetIO(&gios);

		if (world){
			world->globaltime = globaltime;
			world->gios = gios;
			world->Render();
		}

		if (gui){
			gui->globaltime = globaltime;
			int16_t fid = gui->gios.focus;
			gui->gios = gios;
			gui->gios.focus = fid;
			gui->UIInteraction = false;
			gui->Render(&gui->WidgetOrder, true);
			if (!gui->UIInteraction && gios.lastClick == 1)
				gui->gios.focus = -1;
		}

		DrawTextureEx(*cursor.frames[cursor.index], gios.MousePos, 0.0f, 1.0f, WHITE);

		DrawFPS(0, 16);
		DrawText(GetMemoryUsage().c_str(), 0, 0, 20, LIGHTGRAY);
		EndDrawing();
	}

	Quit(this);
	CloseAudioDevice();
	CloseWindow();
}
