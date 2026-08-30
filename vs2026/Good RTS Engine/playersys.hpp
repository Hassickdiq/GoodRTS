#pragma once
#include "main.hpp"
#include "raylib.h"
#include "entitysys.hpp"
#include "Iosys.hpp"

enum class DiplomacyState { Ally, Neutral, Enemy };

struct PlayerResources {
    u16 current_pop = 0;
    u16 max_pop = 20;
};

class Player {
public:
    u8 id;
    const char* name;
    u8 color_id;
    u8 civilization_id;
    Vector2 cam_origin;

    u8 selectCounter = 0;
    int16_t SelectedEnt[SELECT_LIMIT];
    GameIO gios;
    PlayerResources resources;
    DiplomacyState diplomacy[8];
    std::vector<u8> visible_tiles;

    void AddSelectEnt(int16_t entId, void* myWorldptr);
    void SubSelectEnt(int16_t entId, void* myWorldptr);
    void RstSelectEnt(void* myWorldptr);
    void GiveOrderEnt(Action Order, void* myWorldptr);

    void Update(void* myWorldptr, float dt);
};