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
#include "raylib.h"
#include "main.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>

struct Anim2D{
	std::vector<Texture2D*> frames;
	int16_t index = 0;
	double fps = 30.0;
	double time = 0.0;
};

struct TpTile {
	std::string animPath;
	int8_t offX = 0;
	int8_t offY = 0;
};

struct TpEntity {
	std::vector<std::string> animPath;
	std::vector<std::string> soundPath;
	std::string luaPrefix;
	int startFrame = 0;
	float size;
	bool isAnchor;
	u16 maxHitPoint;
	float attackRadius = 0.0f;
	float maxDamage = 0.0f;
	float minDamage = 0.0f;
	bool isThere = false;
	uint16_t eClass = 0;
};

class GameCache {
public:
	std::unordered_map<std::string, Texture2D> Textures;
	std::unordered_map<std::string, Anim2D> Animations;
	std::unordered_map<std::string, Sound> Sounds;
	std::unordered_map<std::string, TpTile> TpTiles;
	std::unordered_map<std::string, TpEntity> TpEntitys;
	std::unordered_map<std::string, bool> Files;

	void UnloadAll();
	void Load(std::string path, lua_State* L = nullptr);
	void CHCInterpreter(std::string name, lua_State* L = nullptr);
	void ENTInterpreter(std::string name);
	void TLPInterpreter(std::string name);
	void PSAInterpreter(std::string name);	
};
