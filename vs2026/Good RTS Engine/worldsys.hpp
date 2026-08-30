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
#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>
#include "raylib.h"
#include "main.hpp"
#include "playersys.hpp"
#include "entitysys.hpp"
#include "chacesys.hpp"
#include "flowmapsys.hpp"
#include "Iosys.hpp"

struct ScxTile {
    Anim2D animation;
    u8 elevation;  // Yükseklik seviyesi
    int8_t offx = 0;
    int8_t offy = 0;
};

Vector2 MathGrid(int x, int y);

Vector2 WorldToGrid(Vector2 pos);

class GameWorld;

class GameMap {
public:
    u32 width;
    u32 height;
    u64 size;
    ScxTile* data;

    GameMap(u32 w, u32 h);

    ~GameMap();

    ScxTile* Get(u32 x, u32 y);

    void Set(ScxTile tile, u32 x, u32 y);

    int Render(GameCache* Cache, Vector2 Cam);
};

class GameWorld {
public:
    std::unordered_map<u64, FlowMap*> FlowMaps;
    std::vector<u64> FlowMapStack;

    GameCache Cache;
    GameIO gios;
    GameMap* map = nullptr;

    bool fogWar = false;
    double globaltime = 0.0;
	Entity* EntityList[ENTITY_LIMIT];
    int16_t EntityOrder[ENTITY_LIMIT];
    std::vector<int> EntityIndexStack;

    lua_State* L = nullptr;

	Player players[9];
	u8 l_plr_id;

    GameWorld(u32 mapW, u32 mapH, u8 plr_id, lua_State* mainL);
    void Delete();
    int Render();

    u64 NewFlowMap(Vector2 targetGridPos);

    int16_t EntitySelect(Vector2 point, u8 plr_id);
    Entity* MakeEntity(TpEntity* tpE, Vector2 origin, u8 player);
	void ResetEntityVector();
	void PushEntity(Entity* entity);
    void PopEntity(int index);

    static int LuaMakeEntity(lua_State* L);
    static int LuaPlayAnimation(lua_State* L);
    static int LuaPlaySound(lua_State* L);
    static int LuaGiveOrder(lua_State* L);
    static int EntityNewIndex(lua_State* L);
    static int EntityIndex(lua_State* L);
};
