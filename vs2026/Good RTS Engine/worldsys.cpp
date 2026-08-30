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

#include "main.hpp"
#include "raylib.h"
#include "raymath.h"
#include "worldsys.hpp"

using namespace std;

// World Class
void GameWorld::ResetEntityVector() {
    EntityIndexStack.clear();
    EntityIndexStack.reserve(ENTITY_LIMIT);
    for (size_t i = (size_t)ENTITY_LIMIT; i != 0; i--) {
        EntityIndexStack.push_back(i - 1);
    }
}

void GameWorld::PushEntity(Entity* entity) {
    if (EntityIndexStack.empty()) {
        throw std::runtime_error("There is not enough space!");
    }

    int index = EntityIndexStack.back();
    EntityIndexStack.pop_back();
    entity->id = index;
    EntityList[index] = entity;

    if (E_DEBUG)
        cout << "pushed: " << index << endl;
}

void GameWorld::PopEntity(int index) {
    if (index < 0 || index >= ENTITY_LIMIT) {
        throw std::out_of_range("Invalid index!");
    }

    if (EntityList[index]) {
        Entity* ent = EntityList[index];
        delete ent->logic.dynamic_vector;

        for (int p = 0; p < 9; p++) {
            if (ent->choseMe[p] != -1) {
                ent->states.isSelected = false;
                players[p].SelectedEnt[ent->choseMe[p]] = -1;
            }
        }

        if (ent->logic.moving) {
            ent->logic.moving = false;
            auto it = FlowMaps.find(ent->logic.targetpos);
            if (it != FlowMaps.end() && it->second != nullptr) {
                it->second->refCount--;
            }
        }

        delete ent;
        EntityList[index] = nullptr;
        for (int i = ENTITY_LIMIT - 1; i > -1; i--) {
            Entity* ent = EntityList[EntityOrder[i]];
            EntityOrder[i] = -1;
            break;
        }
        EntityIndexStack.push_back(index);

        if (E_DEBUG)
            cout << "removed: " << index << endl;
    }
}

Entity* GameWorld::MakeEntity(TpEntity* tpE, Vector2 origin, u8 player) {
    if (tpE->isThere){
        Entity* a = new Entity();
        for (string path : tpE->animPath) {
            a->graphic.Animations.push_back(Cache.Animations[path]);
        }
        for (string path : tpE->soundPath) {
            a->audio.Sounds.push_back(&Cache.Sounds[path]);
        }
        a->states.isAnchor = tpE->isAnchor;
        a->transform.size = tpE->size;
        a->logic.luaprefix = tpE->luaPrefix;
        a->transform.origin = origin;
        a->logic.hitPoint = tpE->maxHitPoint;
        a->logic.maxHitPoint = tpE->maxHitPoint;
        a->logic.dynamic_vector = new vector<double>;
        a->logic.maxDamage = tpE->maxDamage;
        a->logic.minDamage = tpE->minDamage;
        a->logic.attackRadius = tpE->attackRadius;
        a->eClass = tpE->eClass;
        a->ownerID = player;
        PushEntity(a);
        return a;
    }
    cout << "nof found TpEntity" << endl;
    return nullptr;
}

int16_t GameWorld::EntitySelect(Vector2 point, u8 plr_id) {
    for (int i = 0; i < ENTITY_LIMIT; i++) {
        Entity* ent = EntityList[i];
        if (ent) {
            if (ent->id < 0 || ent->id >= ENTITY_LIMIT) continue;

            if (ent->Selectbbox.min.x < point.x && ent->Selectbbox.max.x > point.x &&
                ent->Selectbbox.min.y < point.y && ent->Selectbbox.max.y > point.y) {
                if (E_DEBUG)
                    cout << "selected: " << ent->id << "|" << endl;
                return ent->id;
            }
        }
    }
    if (E_DEBUG)
        cout << "was not selected" << endl;
    return -1;
}

int GameWorld::LuaPlayAnimation(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "MyGameWorldInstance");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
        int16_t wId = (int16_t)lua_tointeger(L, 1);
        int16_t anim = (int16_t)lua_tointeger(L, 2);

        if (wId >= 0 && wId < ENTITY_LIMIT) {
            Entity* ent = myWorld->EntityList[wId];

            ent->graphic.nowAnimation = anim;
            int16_t* index = &ent->graphic.Animations[anim].index;

            if (lua_isinteger(L, 3)) {
                *index = (int16_t)lua_tointeger(L, 3);
            }
            else {
                (*index)++;
            }

            if (*index >= (int16_t)ent->graphic.Animations[anim].frames.size()) {
                *index = 0;
            }

            lua_pushinteger(L, *index);
            return 1;
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}

int GameWorld::LuaGiveOrder(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "MyGameWorldInstance");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (lua_isinteger(L, 1) && lua_isstring(L, 2)) {
        int16_t entId = lua_tointeger(L, 1);
        string Order = lua_tostring(L, 2);

        if (Order == "ATTACK") {
            if (lua_isnumber(L, 3)){
                if (myWorld->EntityList[entId]){
                    Action Attack;
                    Attack.type = ATTACK;
                    Attack.Data[0] = lua_tointeger(L, 3);
                    Attack.Target_id = myWorld->EntityList[entId]->ownerID;
                    Attack.globaltime = myWorld->globaltime;
                    myWorld->EntityList[entId]->nowAction = Attack;
                }
            }
        }
    }
    return 0;
}

int GameWorld::LuaPlaySound(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "MyGameWorldInstance");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (lua_isinteger(L, 1) && lua_isinteger(L, 2)) {
        int16_t wId = (int16_t)lua_tointeger(L, 1);
        int16_t sound = (int16_t)lua_tointeger(L, 2);

        if (wId >= 0 && wId < ENTITY_LIMIT) {
            Entity* ent = myWorld->EntityList[wId];

            if (sound < ent->audio.Sounds.size()){
                ent->audio.nowSound = sound;
                PlaySound(*ent->audio.Sounds[sound]);
            }
            return 0;
        }
    }
    return 0;
}

int GameWorld::LuaMakeEntity(lua_State* L) {
    lua_getfield(L, LUA_REGISTRYINDEX, "MyGameWorldInstance");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isinteger(L, 4)) {
        string tpEntity = lua_tostring(L, 1);
        Vector2 origin = { lua_tonumber(L, 2), lua_tonumber(L, 3) };

        Entity* ent = myWorld->MakeEntity(&myWorld->Cache.TpEntitys[tpEntity], origin, (u8)lua_tointeger(L, 4));

        lua_newtable(L);
        lua_pushinteger(L, ent->id);
        lua_setfield(L, -2, "id");
        luaL_getmetatable(L, "EntityMeta");
        lua_setmetatable(L, -2);
        return 1;
    }

    lua_pushinteger(L, 0);
    return 1;
}

int GameWorld::EntityIndex(lua_State* L) {
    luaL_getmetatable(L, "EntityMeta");
    lua_getfield(L, -1, "__world");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 2);

    lua_getfield(L, 1, "id");
    int id = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    Entity** EntityList = myWorld->EntityList;

    if (EntityList[id]) {
        if (lua_isnumber(L, 2)) {
            long long raw_index = lua_tointeger(L, 2);

            if (raw_index < 0) {
                return luaL_error(L, "Index cannot be negative!");
            }

            size_t index = (size_t)raw_index;
            auto* dyn_vec = EntityList[id]->logic.dynamic_vector;
            if (!dyn_vec) {
                lua_pushnumber(L, 0);
                return 1;
            }
            std::vector<double>& vec = *dyn_vec;

            if (index <= vec.size()) {
                lua_pushnumber(L, vec[index]);
                return 1;
            }
            lua_pushnumber(L, 0);
            return 1;
        }

        const char* key = lua_tostring(L, 2);

        size_t keycode = 0;
        if (key[0] == 0) return 0;

        for (size_t i = 0; i < strlen(key); i++) {
            keycode += ((size_t)key[i] * (i + (size_t)key[i]));
        }
        keycode *= (keycode & (keycode + strlen(key) / key[0]));

        switch (keycode)
        {
        case 51872795536: { // transform_origin_x
            lua_pushnumber(L, static_cast<float>(EntityList[id]->transform.origin.x));
            return 1;
        }
        case 51990384196: { // transform_origin_y
            lua_pushnumber(L, static_cast<float>(EntityList[id]->transform.origin.y));
            return 1;
        }
        case 57142990116: { // trasnform_direction
            lua_pushinteger(L, static_cast<int>(EntityList[id]->transform.currentDirection));
            return 1;
        }
        case 31339266841: { // trasnform_size
            lua_pushinteger(L, static_cast<int>(EntityList[id]->transform.size));
            return 1;
        }
        case 39116532841: { // transform_angles
            lua_pushinteger(L, static_cast<int>(EntityList[id]->transform.angles));
            return 1;
        }
        case 29232450625: { // logic_nexttime
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.nexttime));
            return 1;
        }
        case 58923193081: { // logic_nexttimeAttack
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.nexttimeAttack));
            return 1;
        }
        case 58094014729: { // logic_attackDuration
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.nexttimeAttack));
            return 1;
        }
        case 27108634609: { // logic_hitPoint
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.hitPoint));
            return 1;
        }
        case 27573930916: { // logic_minDamage
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.minDamage));
            return 1;
        }
        case 27809564644: { // logic_maxDamage
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.maxDamage));
            return 1;
        }
        case 30060971161: { // logic_moveSpeed
            lua_pushnumber(L, static_cast<float>(EntityList[id]->logic.moveSpeed));
            return 1;
        }
        case 58744186384: { // graphic_nowAnimation
            lua_pushnumber(L, static_cast<int>(EntityList[id]->graphic.nowAnimation));
            return 1;
        }
        case 33606589041: { // states_dontMove
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.dontMove));
            return 1;
        }
        case 39874099225: { // states_canItTouch
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.canItTouch));
            return 1;
        }
        case 28824569284: { // states_canItSee
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.canItSee));
            return 1;
        }
        case 54195840000: { // interactElev
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.interactElev));
            return 1;
        }
        case 36689870116: { // states_isNoticed
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.isNoticed));
            return 1;
        }
        case 43896535225: { // states_isCollison
            lua_pushboolean(L, static_cast<int>(EntityList[id]->states.isCollison));
            return 1;
        }
        case 4073247684: { // eClass
            lua_pushnumber(L, static_cast<int>(EntityList[id]->eClass));
            return 1;
        }
        case 5417401609: { // ownerID
            lua_pushinteger(L, static_cast<int>(EntityList[id]->ownerID));
            return 1;
        }
        case 11091670489: { // nowAction
            lua_pushinteger(L, static_cast<int>(EntityList[id]->nowAction.type));
            return 1;
        }
        default:
            cout << "Unknown key | " << key << ": " << keycode << endl;
            break;
        }
    }
    return 0;
}

int GameWorld::EntityNewIndex(lua_State* L) {
    luaL_getmetatable(L, "EntityMeta");
    lua_getfield(L, -1, "__world");
    GameWorld* myWorld = (GameWorld*)lua_touserdata(L, -1);
    lua_pop(L, 2);

    lua_getfield(L, 1, "id");
    int id = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);

    Entity** EntityList = myWorld->EntityList;

    if (EntityList[id]) {

        if (lua_isnumber(L, 2)) {
            long long raw_index = lua_tointeger(L, 2);

            if (raw_index < 0) {
                return luaL_error(L, "Index cannot be negative!");
            }

            size_t index = (size_t)raw_index;

            auto* dyn_vec = EntityList[id]->logic.dynamic_vector;
            if (!dyn_vec) return 0;

            std::vector<double>& vec = *dyn_vec;

            if (index >= vec.size()) {
                vec.resize(index + 1, 0);
            }

            vec[index] = lua_tonumber(L, 3);
            return 0;
        }

        const char* key = lua_tostring(L, 2);

        size_t keycode = 0;
        if (key[0] == 0) return 0;

        for (size_t i = 0; i < strlen(key); i++) {
            keycode += ((size_t)key[i] * (i + (size_t)key[i]));
        }
        keycode *= (keycode & (keycode + strlen(key) / key[0]));

        switch (keycode)
        {
        case 51872795536: { // transform_origin_x
            EntityList[id]->transform.origin.x = static_cast<float>(lua_tonumber(L, 3));
            EntityList[id]->Rendered = false;
            return 0;
        }
        case 51990384196: { // transform_origin_y
            EntityList[id]->transform.origin.y = static_cast<float>(lua_tonumber(L, 3));
            EntityList[id]->Rendered = false;
            return 0;
        }
        case 57142990116: { // transform_direction
            EntityList[id]->transform.currentDirection = static_cast<int>(lua_tonumber(L, 3));
            return 0;
        }
        case 31339266841: { // transform_size
            EntityList[id]->transform.size = static_cast<int>(lua_tonumber(L, 3));
            return 0;
        }
        case 39116532841: { // transform_angles
            EntityList[id]->transform.angles = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 29232450625: { // logic_nexttime
            EntityList[id]->logic.nexttime = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 58923193081: { // logic_nexttimeAttack
            EntityList[id]->logic.nexttimeAttack = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 58094014729: { // logic_attackDuration
            EntityList[id]->logic.attackDuration = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 27573930916: { // logic_minDamage
            EntityList[id]->logic.minDamage = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 27809564644: { // logic_maxDamage
            EntityList[id]->logic.maxDamage = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 27108634609: { // logic_hitPoint
            EntityList[id]->logic.hitPoint = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 30060971161: { // logic_moveSpeed
            EntityList[id]->logic.moveSpeed = static_cast<float>(lua_tonumber(L, 3));
            return 0;
        }
        case 33606589041: { // states_dontMove
            EntityList[id]->states.dontMove = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 39874099225: { // states_canItTouch
            EntityList[id]->states.canItTouch = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 28824569284: { // states_canItSee
            EntityList[id]->states.canItSee = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 54195840000: { // states_interactElev
            EntityList[id]->states.interactElev = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 36689870116: { // states_isNoticed
            EntityList[id]->states.isNoticed = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 43896535225: { // states_isCollison
            EntityList[id]->states.isCollison = static_cast<bool>(lua_toboolean(L, 3));
            return 0;
        }
        case 5417401609: { // ownerID
            EntityList[id]->ownerID = static_cast<bool>(lua_tointeger(L, 3));
            return 0;
        }
        default:
            cout << "Unknown key | " << key << ": " << keycode << endl;
            break;
        }
    }
    return 0;
}

GameWorld::GameWorld(u32 mapW, u32 mapH, u8 plr_id, lua_State* mainL) {
    map = new GameMap(mapW, mapH);
    for (u64 grid : FlowMapStack) {
        if (FlowMaps[grid]){
            delete[] FlowMaps[grid]->map;
            delete FlowMaps[grid];
            FlowMaps[grid] = nullptr;
        }
    }

    L = mainL;
    l_plr_id = plr_id;
    luaL_newmetatable(L, "EntityMeta");

    lua_pushlightuserdata(L, this);
    lua_setfield(L, -2, "__world");

    lua_pushcfunction(L, EntityIndex);
    lua_setfield(L, -2, "__index");

    lua_pushcfunction(L, EntityNewIndex);
    lua_setfield(L, -2, "__newindex");

    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, "MyGameWorldInstance");

    lua_pushcfunction(L, GameWorld::LuaPlayAnimation);
    lua_setglobal(L, "PlayAnimation");

    lua_pushcfunction(L, GameWorld::LuaPlaySound);
    lua_setglobal(L, "PlaySound");

    lua_pushcfunction(L, GameWorld::LuaMakeEntity);
    lua_setglobal(L, "MakeEntity");

    lua_pushcfunction(L, GameWorld::LuaGiveOrder);
    lua_setglobal(L, "GiveOrder");

    ResetEntityVector();
}

void GameWorld::Delete(){
    if (map) {
        delete map;
        map = nullptr;
    }
    if (L != nullptr) {
        lua_close(L);
        L = nullptr;
    }
    for (int i = 0; i < ENTITY_LIMIT; i++) {
        if (EntityList[i]) {
            delete EntityList[i]->logic.dynamic_vector;
            delete EntityList[i];
            EntityList[i] = nullptr;
        }
    }
    Cache.UnloadAll();
}

u64 GameWorld::NewFlowMap(Vector2 targetGridPos) {
    int targetX = static_cast<int>(targetGridPos.x);
    int targetY = static_cast<int>(targetGridPos.y);

    u64 grid = targetY * map->width + targetX;

    if (!FlowMaps[grid]){
        FlowMaps[grid] = new FlowMap(this, targetGridPos);
        FlowMaps[grid]->time = globaltime;
        FlowMapStack.push_back(grid);
    }

    return grid;
}

int GameWorld::Render() {
    float dt = GetFrameTime();
    if (dt > 0.05f) dt = 0.05f;

    if (!FlowMapStack.empty()) {
        u64 lastmap = FlowMapStack.back();
        auto it = FlowMaps.find(lastmap);
        if (it != FlowMaps.end() && it->second != nullptr) {
            if (it->second->time + 180 < globaltime || it->second->refCount == 0) {
                delete[] it->second->map;
                delete it->second;
                FlowMaps.erase(it);
                FlowMapStack.pop_back();  
            }
        }
        else {
            FlowMapStack.pop_back();
        }
    }

    int orderCount = 0;
    for (int i = 0; i < ENTITY_LIMIT; i++) {
        Entity* ent = EntityList[i];
        if (ent) {
            EntityOrder[orderCount++] = i;
        }
    }

    std::sort(EntityOrder, EntityOrder + orderCount, [this](int16_t a, int16_t b) {
        Entity* entA = EntityList[a];
        Entity* entB = EntityList[b];

        if (!entA || !entB) return false;

        return entA->transform.origin.y < entB->transform.origin.y;
    });

    Player* localPlayer = &players[l_plr_id];
    localPlayer->gios = gios;
    Vector2 cam = localPlayer->cam_origin;
    localPlayer->Update((void*)this, dt);

    map->Render(&Cache, cam);

    for (int i = 0; i < orderCount; i++) {
        Entity* ent = EntityList[EntityOrder[i]];

        if (ent)
        {      
            if (ent->states.died){
                PopEntity(ent->id);
                continue;
            }

            ent->Update(this, dt);

            Anim2D* Anim = &ent->graphic.Animations[ent->graphic.nowAnimation];
            Texture2D tx = *Anim->frames[Anim->index];

            if (ent->states.isSelected)
            {
                float maxHitPoint = static_cast<float>(ent->logic.maxHitPoint);
                float hitPoint = static_cast<float>(ent->logic.hitPoint);
                Vector2 hpBarOrigin = { ent->transform.origin.x + cam.x - 20,  ent->transform.origin.y + -16 * ent->transform.elevation + cam.y - tx.height };

                if (IsKeyDown('A')) {
                    DrawEllipseLines(ent->transform.origin.x + cam.x, ent->transform.origin.y + -16 * ent->transform.elevation + cam.y + (tx.height / 2.0f) - 5, ent->logic.attackRadius, ent->logic.attackRadius / 2, RED);
                    DrawEllipseLines(ent->transform.origin.x + cam.x, ent->transform.origin.y + -16 * ent->transform.elevation + cam.y + (tx.height / 2.0f) - 5, ent->logic.fowRadius, ent->logic.fowRadius / 2, DARKBLUE);
                }

                DrawEllipseLines(ent->transform.origin.x + cam.x, ent->transform.origin.y + cam.y + 10 + (-16 * ent->transform.elevation), ent->transform.size, ent->transform.size / 2, WHITE);
                DrawRectangleV(hpBarOrigin, { 40, 5 }, BLACK);
                hpBarOrigin.x += 1;
                hpBarOrigin.y += 1;
                DrawRectangleV(hpBarOrigin, { (38.0f / maxHitPoint) * hitPoint, 3 }, DARKGREEN);
            }

            Rectangle sourceRec = { 0.0f, 0.0f, (float)tx.width, (float)tx.height };
            Rectangle destRec = {
                ent->transform.origin.x + cam.x - tx.width / 2.0f,
                ent->transform.origin.y + -16 * ent->transform.elevation + cam.y - tx.height / 2.0f,
                (float)tx.width,
                (float)tx.height
            };
            Vector2 origin = { 0.0f, 0.0f };

            uint8_t dir = ent->transform.currentDirection;

            if (dir == 1 || dir == 7 || dir == 6 || dir == 0) {
                sourceRec.width = -sourceRec.width;
            }

            if (ent->ownerID == 1)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, BLUE);
            else if (ent->ownerID == 2)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, RED);
            else if (ent->ownerID == 3)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, GREEN);
            else if (ent->ownerID == 5)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, PURPLE);
            else if (ent->ownerID == 3)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, GREEN);
            else if (ent->ownerID == 6)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, SKYBLUE);
            else if (ent->ownerID == 4)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, YELLOW);
            else if (ent->ownerID == 7)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, ORANGE);
            else if (ent->ownerID == 9)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, GRAY);
            else if (ent->ownerID == 7)
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, PINK);
            else
                DrawTexturePro(tx, sourceRec, destRec, origin, 0.0f, WHITE);

            if (E_DEBUG)
                DrawCircleLines(ent->transform.origin.x + cam.x, ent->transform.origin.y + -16 * ent->transform.elevation + cam.y + tx.height / 2.0f, ent->transform.size, RED);
        }
    }

    return -1;
}

// Map Class
GameMap::GameMap(u32 w, u32 h) {
    width = w;
    height = h;
    size = (u64)(width) * (u64)(height);
    data = new ScxTile[width * height];
}

GameMap::~GameMap() {
    delete[] data;
}

ScxTile* GameMap::Get(u32 x, u32 y) {
    if (x < width && y < height)
        return &data[(y * width) + x];
    return &data[0];
}

void GameMap::Set(ScxTile tile, u32 x, u32 y) {
    if (tile.elevation > 7) tile.elevation = 7;

    if (x < width && y < height) {
        u32 grid = (y * width) + x;
        data[grid] = tile;
    }
}

Vector2 MathGrid(int x, int y) {
    Vector2 rpos;
    rpos.x = (x - y) * (TILE_WIDTH / 2);
    rpos.y = (x + y) * (TILE_HEIGHT / 2);
    return rpos;
}

Vector2 WorldToGrid(Vector2 pos) {
    Vector2 gridPos;

    float X = pos.x / (TILE_WIDTH / 2.0f);
    float Y = pos.y / (TILE_HEIGHT / 2.0f);

    gridPos.x = (Y + X) / 2.0f;
    gridPos.y = (Y - X) / 2.0f;
    return gridPos;
}

int GameMap::Render(GameCache* Cache, Vector2 Cam) {
    ScxTile* tile = nullptr;
    u32 mapWidth = width;
    u32 mapHeight = height;
    u64 mapSize = mapHeight * mapWidth;

    for (u32 y = 0; y < mapHeight; y++) {
        for (u32 x = 0; x < mapWidth; x++) {
            tile = Get(x, y);

            Vector2 pos = MathGrid(x, y);
            
            if (Cam.x + 50 > -pos.x - 100 && Cam.x - 50 < -pos.x + SCREEN_WIDTH && Cam.y + 50 > -pos.y && Cam.y - 50 < -pos.y + SCREEN_HEIGHT) {
                Vector2 drwp = { Cam.x + pos.x + tile->offx, Cam.y + pos.y - tile->elevation * 16 + tile->offy };

                if (tile->animation.index == 16 || tile->animation.index == 10
                    || tile->animation.index == 14 || tile->animation.index == 23
                    || tile->animation.index == 22 || tile->animation.index == 24)
                    drwp.y -= 16;

                if (!tile->animation.frames.empty())
                    DrawTextureEx(*tile->animation.frames[tile->animation.index], drwp, 0, 1.0f, WHITE);
            }
        }
    }

    return 1;
}
