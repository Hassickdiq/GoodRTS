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

#include "entitysys.hpp"
#include "worldsys.hpp"

using namespace std;

void Entity::SpawnEntity(lua_State* L, double globaltime) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Spawn").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, globaltime);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::DiedEntity(lua_State* L) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Died").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::ThinkEntity(lua_State* L, double gloabltime) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Think").c_str());

	if (lua_isfunction(L, -1)) {

		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, gloabltime);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::MoveEntity(lua_State* L, Vector2 target) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Move").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		lua_pushinteger(L, (int)(target.x));
		lua_pushinteger(L, (int)(target.y));

		if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::StopEntity(lua_State* L, Vector2 target) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Stop").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		lua_pushinteger(L, (int)(target.x));
		lua_pushinteger(L, (int)(target.y));

		if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::AroundMeEntity(lua_State* L, Entity* targetEntity) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "AroundMe").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_pushinteger(L, targetEntity->id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::AttackEntity(lua_State* L, Entity* targetEntity, double globaltime) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Attack").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_pushinteger(L, targetEntity->id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, globaltime);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::FollowEntity(lua_State* L, Entity* targetEntity) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Follow").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_pushinteger(L, targetEntity->id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::TouchEntity(lua_State* L, Entity* touchEntity) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Touch").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_pushinteger(L, touchEntity->id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::SawEntity(lua_State* L, float distance, Entity* touchEntity) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	if (!this) return;

	lua_getglobal(L, (logic.luaprefix + "Saw").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, distance);

		lua_newtable(L);
		lua_pushinteger(L, touchEntity->id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "EntityMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 4, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {

		lua_pop(L, 1);
	}
}

void Entity::Update(void* myWorldptr, float dt) {
	GameWorld* myWorld = (GameWorld*)myWorldptr;

	if (!states.isSpawned) {
		SpawnEntity(myWorld->L, myWorld->globaltime);
		states.isSpawned = true;
	}

	if (logic.nexttime < myWorld->globaltime) {
		logic.nexttime = myWorld->globaltime + 0.1;
		ThinkEntity(myWorld->L, myWorld->globaltime);
	}

	if (!Rendered)
	{	
		Anim2D* Anim = &graphic.Animations[graphic.nowAnimation];
		Texture2D* tx = Anim->frames[Anim->index];

		// for Select
		Selectbbox.max.x = transform.origin.x + tx->width / 2;
		Selectbbox.min.x = transform.origin.x - tx->width / 2;

		Selectbbox.max.y = transform.origin.y + (tx->height / 2) - (16 * transform.elevation);;
		Selectbbox.min.y = transform.origin.y - (tx->height / 2) - (16 * transform.elevation);

		Vector2 collTile = WorldToGrid(transform.origin);
		u64 grid = static_cast<int>(collTile.y) * myWorld->map->width + static_cast<int>(collTile.x);
		if (grid < myWorld->map->size) {
			transform.elevation = myWorld->map->data[grid].elevation;
		}

		for (int i = 0; i < ENTITY_LIMIT; i++) {
			Entity* entA = myWorld->EntityList[i];
			Entity* entB = this;

			if (entA && entA != entB) {

				float distX = entA->transform.origin.x - entB->transform.origin.x;
				float distY = entA->transform.origin.y - entB->transform.origin.y;

				float adjustedDistY = distY * 2.0f;

				float distance = sqrt((distX * distX) + (adjustedDistY * adjustedDistY));

				float combinedRadius = entA->transform.size + entB->transform.size;

				if (distance < combinedRadius && distance > 0.0f) {

					if (entB->states.canItTouch) {
						TouchEntity(myWorld->L, entA);
					}

					float overlap = combinedRadius - distance;

					float dirX = distX / distance;
					float dirY = adjustedDistY / distance;

					float pushForce = overlap * 0.5f * dt * 50.0f;
					float pushX = dirX * pushForce;
					float pushY = (dirY * 0.5f) * pushForce;

					if (!entA->states.isAnchor && states.isCollison) {
						Vector2 newPosA = { entA->transform.origin.x + pushX, entA->transform.origin.y + pushY };
						Vector2 gridPosA = WorldToGrid(newPosA);
						int gxa = (int)(gridPosA.x + 0.5f);
						int gya = (int)(gridPosA.y + 0.5f);

						if (gxa >= 0 && gxa < myWorld->map->width && gya >= 0 && gya < myWorld->map->height) {
							if (!myWorld->map->data[gya * myWorld->map->width + gxa].isBlocked) {
								entA->transform.origin = newPosA;
								entA->Rendered = false;
							}
						}
					}

					if (!entB->states.isAnchor && states.isCollison) {
						float multiplier = (entA->states.isAnchor) ? 2.0f : 1.0f;
						Vector2 newPosB = { entB->transform.origin.x - (pushX * multiplier), entB->transform.origin.y - (pushY * multiplier) };
						Vector2 gridPosB = WorldToGrid(newPosB);
						int gxb = (int)(gridPosB.x + 0.5f);
						int gyb = (int)(gridPosB.y + 0.5f);

						if (gxb >= 0 && gxb < myWorld->map->width && gyb >= 0 && gyb < myWorld->map->height) {
							if (!myWorld->map->data[gyb * myWorld->map->width + gxb].isBlocked) {
								entB->transform.origin = newPosB;
							}
						}
					}
				}
			}
		}

		Rendered = true;
	}

	if (states.canItSee)
		if (logic.nexttimeSaw < myWorld->globaltime){
			logic.nexttimeSaw = myWorld->globaltime + 1.0;

			for (int i = 0; i < ENTITY_LIMIT; i++) {
				Entity* entB = this;
				Entity* entA = myWorld->EntityList[i];

				if (entA && entA != entB) {
					if (entA->states.isNoticed)
					{
						float distX = entA->transform.origin.x - entB->transform.origin.x;
						float distY = entA->transform.origin.y - entB->transform.origin.y;

						float radiusX = logic.fowRadius;
						float radiusY = logic.fowRadius * 0.5f;

						float normalizedDist = (distX * distX) / (radiusX * radiusX) + (distY * distY) / (radiusY * radiusY);

						if ((normalizedDist < 1.0f)) {
							SawEntity(myWorld->L, normalizedDist, entA);
						}
					}
				}
			}
		}

	if (nowAction.type == DIE || logic.hitPoint <= 0) {
		DiedEntity(myWorld->L);
		states.died = true;
	}
	else if (nowAction.type == ATTACK) {
		int16_t entId = nowAction.Data[0];

		if (myWorld->EntityList[entId] != this && myWorld->EntityList[entId]) {
			Entity* entB = this;
			Entity* entA = myWorld->EntityList[entId];
			Vector2 targetEntity = entA->transform.origin;
			Vector2 targetgridPos = WorldToGrid(targetEntity);

			float distX = entA->transform.origin.x - transform.origin.x;
			float distY = entA->transform.origin.y - transform.origin.y;

			float radiusX = logic.attackRadius;
			float radiusY = logic.attackRadius * 0.5f;

			float normalizedDist = (distX * distX) / (radiusX * radiusX) + (distY * distY) / (radiusY * radiusY);

			float adjustedDistY = distY * 2.0f;

			float distance = sqrt((distX * distX) + (adjustedDistY * adjustedDistY));

			float combinedRadius = entA->transform.size + entB->transform.size;

			if ( normalizedDist > 1.0f && !(distance < combinedRadius && distance > 0.0f)) {
				if (!states.dontMove)
				{
					u64 flowId = myWorld->NewFlowMap(targetgridPos);

					Vector2 gridPos = WorldToGrid(transform.origin);
					int gx = (int)(gridPos.x + 0.5f);
					int gy = (int)(gridPos.y + 0.5f);

					if (gx >= 0 && gx < myWorld->map->width && gy >= 0 && gy < myWorld->map->height && myWorld->FlowMaps[flowId]) {
						if (nowAction.globaltime == myWorld->globaltime) {
							if (logic.moving) {
								auto it = myWorld->FlowMaps.find(logic.targetpos);
								if (it != myWorld->FlowMaps.end() && it->second != nullptr) {
									it->second->refCount--;
								}
							}
							logic.moving = false;
						}

						if (!logic.moving) {
							logic.targetpos = flowId;
							myWorld->FlowMaps[flowId]->refCount++;
							logic.moving = true;
							if (myWorld->EntityList[entId] == entA)
								FollowEntity(myWorld->L, entA);
						}

						Vector2 flowDir = myWorld->FlowMaps[flowId]->map[gy * myWorld->map->width + gx].direction;

						if (flowDir.x != 0 || flowDir.y != 0) {
							Vector2 point = transform.origin;

							float worldDirX = (flowDir.x - flowDir.y) * (TILE_WIDTH / 2.0f);
							float worldDirY = (flowDir.x + flowDir.y) * (TILE_HEIGHT / 2.0f);

							float length = sqrt(worldDirX * worldDirX + worldDirY * worldDirY);
							if (length > 0.0f) {
								worldDirX /= length;
								worldDirY /= length;
							}

							float speed = logic.moveSpeed;
							transform.origin.x += worldDirX * speed * dt;
							transform.origin.y += worldDirY * speed * dt;

							float diffX = transform.origin.x - point.x;
							float diffY = transform.origin.y - point.y;
							float distance = sqrt((diffX * diffX) + (diffY * diffY));

							float dirX = diffX / distance;
							float dirY = diffY / distance;

							float angle = atan2(diffY, diffX);

							float degrees = angle * (180.0f / 3.14159265f);
							if (degrees < 0) degrees += 360.0f;

							int directionIndex = static_cast<int>((degrees + 22.5f) / 45.0f) % 8;
							transform.angles = degrees;
							transform.currentDirection = (uint8_t)directionIndex;
						}
						Rendered = false;
					}
				}
			}
			else {
				entA = myWorld->EntityList[entId];
				if (logic.moving) {
					logic.moving = false;
					auto it = myWorld->FlowMaps.find(logic.targetpos);
					if (it != myWorld->FlowMaps.end() && it->second != nullptr) {
						it->second->refCount--;
						if (entA)
							AroundMeEntity(myWorld->L, entA);
					}
				}
				if (logic.nexttimeAttack < myWorld->globaltime) {
					logic.nexttimeAttack = myWorld->globaltime + 1.5;
					if (entA)
						if ((!states.interactElev || entA->transform.elevation == transform.elevation))
							AttackEntity(myWorld->L, entA, myWorld->globaltime);
				}
			}
		}
		else {
			if (logic.moving) {
				logic.moving = false;
				auto it = myWorld->FlowMaps.find(logic.targetpos);
				if (it != myWorld->FlowMaps.end() && it->second != nullptr) {
					it->second->refCount--;
				}
			}
			nowAction.type = NOP;
			StopEntity(myWorld->L, Vector2(static_cast<float>(nowAction.Data[0]), static_cast<float>(nowAction.Data[1])));
		}
	}
	else if (nowAction.type == MOVE && !states.dontMove) {
		if (nowAction.globaltime == myWorld->globaltime)
			MoveEntity(myWorld->L, Vector2(static_cast<float>(nowAction.Data[0]), static_cast<float>(nowAction.Data[1])));

		Vector2 gridPos = WorldToGrid(transform.origin);
		int gx = (int)(gridPos.x + 0.5f);
		int gy = (int)(gridPos.y + 0.5f);
		int flowId = nowAction.Data[2];

		if (gx >= 0 && gx < myWorld->map->width && gy >= 0 && gy < myWorld->map->height && myWorld->FlowMaps.find(flowId) != myWorld->FlowMaps.end()) {
			if (nowAction.globaltime == myWorld->globaltime) {
				if (logic.moving) {
					auto it = myWorld->FlowMaps.find(logic.targetpos);
					if (it != myWorld->FlowMaps.end() && it->second != nullptr) {
						it->second->refCount--;
					}
				}
				logic.moving = false;
			}

			if (!logic.moving) {
				logic.targetpos = flowId;
				myWorld->FlowMaps[flowId]->refCount++;
				logic.moving = true;
			}

			Vector2 flowDir = myWorld->FlowMaps[flowId]->map[gy * myWorld->map->width + gx].direction;

			if (flowDir.x != 0 || flowDir.y != 0) {
				Vector2 point = transform.origin;

				float worldDirX = (flowDir.x - flowDir.y) * (TILE_WIDTH / 2.0f);
				float worldDirY = (flowDir.x + flowDir.y) * (TILE_HEIGHT / 2.0f);

				float length = sqrt(worldDirX * worldDirX + worldDirY * worldDirY);
				if (length > 0.0f) {
					worldDirX /= length;
					worldDirY /= length;
				}

				float speed = logic.moveSpeed;
				transform.origin.x += worldDirX * speed * dt;
				transform.origin.y += worldDirY * speed * dt;

				float diffX = transform.origin.x - point.x;
				float diffY = transform.origin.y - point.y;
				float distance = sqrt((diffX * diffX) + (diffY * diffY));

				float dirX = diffX / distance;
				float dirY = diffY / distance;

				float angle = atan2(diffY, diffX);

				float degrees = angle * (180.0f / 3.14159265f);
				if (degrees < 0) degrees += 360.0f;

				int directionIndex = static_cast<int>((degrees + 22.5f) / 45.0f) % 8;
				transform.angles = degrees;
				transform.currentDirection = (uint8_t)directionIndex;
			}
			else {
				if (logic.moving) {
					logic.moving = false;
					auto it = myWorld->FlowMaps.find(logic.targetpos);
					if (it != myWorld->FlowMaps.end() && it->second != nullptr) {
						it->second->refCount--;
					}
				}
				nowAction.type = NOP;
				StopEntity(myWorld->L, Vector2(static_cast<float>(nowAction.Data[0]), static_cast<float>(nowAction.Data[1])));
			}
		}
		Rendered = false;
	}
}