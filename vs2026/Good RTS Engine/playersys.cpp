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

#include "playersys.hpp"
#include "worldsys.hpp"

using namespace std;

void Player::AddSelectEnt(int16_t entId, void* myWorldptr) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;

	if (selectCounter < SELECT_LIMIT) {
		Entity* ent = myWorld->EntityList[entId];
		if (ent){
			if (gios.ctrl){
				if(!ent->states.isSelected){
					SelectedEnt[selectCounter] = ent->id;
					ent->states.isSelected = true;
					ent->choseMe[id] = selectCounter;
					selectCounter++;
				}
				else
					SubSelectEnt(entId, myWorldptr);
			}
			else {
				RstSelectEnt(myWorldptr);
				SelectedEnt[selectCounter] = ent->id;
				ent->states.isSelected = true;
				ent->choseMe[id] = selectCounter;
				selectCounter++;
			}
		}
	}
}

void Player::SubSelectEnt(int16_t entId, void* myWorldptr) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;

	for (u8 i = 0; i < SELECT_LIMIT; i++) {
		if (SelectedEnt[i] == -1) continue;
		Entity* ent = myWorld->EntityList[SelectedEnt[i]];

		if (ent)
			if (ent->id == entId) {
				ent->states.isSelected = false;
				ent->choseMe[id] = -1;
				SelectedEnt[i] = -1;
			}
	}
}

void Player::RstSelectEnt(void* myWorldptr) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;
	selectCounter = 0;

	for (u8 i = 0; i < SELECT_LIMIT; i++) {
		if (SelectedEnt[i] == -1) continue;
		Entity* ent = myWorld->EntityList[SelectedEnt[i]];

		if (ent) {
			ent->states.isSelected = false;
			ent->choseMe[id] = -1;
			SelectedEnt[i] = -1;
		}
		else SelectedEnt[i] = -1;
	}
}

void Player::GiveOrderEnt(Action Order, void* myWorldptr) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;

	for (u8 i = 0; i < SELECT_LIMIT; i++) {
		if (SelectedEnt[i] == -1) continue;
		Entity* ent = myWorld->EntityList[SelectedEnt[i]];

		if (ent) {
			ent->nowAction = Order;
		}
	}
}

void Player::Update(void* myWorldptr, float dt) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;

	float cam_speed = 250.0f;

	if (gios.keyUp)
		cam_origin.y += cam_speed * dt;
	if (gios.keyDown)
		cam_origin.y -= cam_speed * dt;
	if (gios.keyRight)
		cam_origin.x -= cam_speed * dt;
	if (gios.keyLeft)
		cam_origin.x += cam_speed * dt;

	if (gios.lastClick == 1){
		int16_t entId = myWorld->EntitySelect({ gios.MousePos.x - cam_origin.x, gios.MousePos.y - cam_origin.y }, id);
		if (entId != -1)
			AddSelectEnt(entId, myWorldptr);
		else
			RstSelectEnt(myWorldptr);
	}
	else if (gios.lastClick == 0 && (gios.ctrl) && IsMouseButtonDown(0)) {
		int16_t entId = myWorld->EntitySelect({ gios.MousePos.x - cam_origin.x, gios.MousePos.y - cam_origin.y }, id);
		if (entId != -1){	
			if (myWorld->EntityList[entId])
				if (myWorld->EntityList[entId]->choseMe[id] == -1)
					AddSelectEnt(entId, myWorldptr);
		}
	}
	else if (gios.lastClick == 2) {
		int16_t entId = myWorld->EntitySelect({ gios.MousePos.x - cam_origin.x, gios.MousePos.y - cam_origin.y }, id);

		if (entId == -1){
			Vector2 clickWorld = { gios.MousePos.x - cam_origin.x, gios.MousePos.y - cam_origin.y };
			Vector2 gridPos = WorldToGrid(clickWorld);

			int gridX = (int)(gridPos.x + 0.5f);
			int gridY = (int)(gridPos.y + 0.5f);

			// Akış alanını hesapla
			int indx = myWorld->NewFlowMap({ (float)gridX, (float)gridY });

			if (indx != -1){
				Action Move;
				Move.type = MOVE;
				Move.Data[0] = gios.MousePos.x - cam_origin.x;
				Move.Data[1] = gios.MousePos.y - cam_origin.y;
				Move.Data[2] = indx;
				Move.Target_id = id;
				Move.globaltime = myWorld->globaltime;

				GiveOrderEnt(Move, myWorldptr);
			}
		}
		else if (myWorld->EntityList[entId]) {
			Action Move;
			Move.type = ATTACK;
			Move.Data[0] = entId;
			Move.Target_id = id;
			Move.globaltime = myWorld->globaltime;

			GiveOrderEnt(Move, myWorldptr);
		}
	}

	if (IsKeyDown(KEY_DELETE)) {
		Action Die;
		Die.type = DIE;
		Die.Target_id = id;
		GiveOrderEnt(Die, myWorldptr);
		RstSelectEnt(myWorldptr);
	}
}
