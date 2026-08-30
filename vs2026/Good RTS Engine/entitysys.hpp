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
#include "chacesys.hpp"

struct eBoundbox{
	Vector2 min;
	Vector2 max;
};

struct eTransform {
	Vector2 origin;
	u8 elevation;
	float angles = 0.0f;
	float size;
	u8 currentDirection = 0;
};

struct eLogic{
	int16_t hitPoint = 0;
	uint16_t maxHitPoint = 0;
	std::string luaprefix;
	std::vector<double>* dynamic_vector = nullptr;
	float attackRadius = 0.0f;
	float fowRadius = 500.0f;
	float maxDamage = 0.0f;
	float minDamage = 0.0f;
	double nexttime = 0.0;
	double nexttimeSaw = 0.0;
	double nexttimeAttack = 0.0;
	float moveSpeed = 50.0f;
	bool moving = false;
	u64 targetpos = 0;
};

struct eGraphic{
	std::vector<Anim2D> Animations;
	int16_t nowAnimation = 0;
};

struct eAudio {
	std::vector<Sound*> Sounds;
	int16_t nowSound = 0;
	float volume = false;
	bool soundLoop = false;
};

struct eStates{
	bool isSelected = false;
	bool isAnchor = false;
	bool isSpawned = false;
	bool died = false;
	bool dontMove = false;
	bool canItTouch = true;
	bool canItSee = true;
	bool interactElev = true;
	bool isNoticed = true;
	bool isCollison = true;
};

enum Event {
	NOP,
	SELECT,
	MOVE,
	ATTACK,
	DIE,
};

class Action {
public:
	Event type = NOP;
	int16_t Target_id = -1;
	int Data[12];
	double globaltime = 0.0;
};

class Entity {
public:
	int16_t choseMe[9] = {-1, -1, -1, -1, -1, -1, -1, -1};
	uint16_t eClass;
	int16_t id = -1;
	u8 ownerID;
	int16_t grupID;
	eStates states;
	eTransform transform;
	eBoundbox boundbox;
	eBoundbox Selectbbox;
	eGraphic graphic;
	eAudio audio;
	eLogic logic;
	Action nowAction;
	bool Rendered = false;

	void Update(void* myWorldptr, float dt);
	void SpawnEntity(lua_State* L, double globaltime);
	void ThinkEntity(lua_State* L, double gloabltime);
	void MoveEntity(lua_State* L, Vector2 target);
	void DiedEntity(lua_State* L);
	void StopEntity(lua_State* L, Vector2 target);
	void AroundMeEntity(lua_State* L, Entity* targetEntity);
	void AttackEntity(lua_State* L, Entity* targetEntity, double globaltime);
	void FollowEntity(lua_State* L, Entity* targetEntity);
	void TouchEntity(lua_State* L, Entity* touchEntity);
	void SawEntity(lua_State* L, float distance, Entity* touchEntity);
};
