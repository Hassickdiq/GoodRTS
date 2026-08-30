#pragma once
#include "raylib.h"
#include <queue>
#include "main.hpp"

struct FlowCell {
    Vector2 direction = { 0.0f, 0.0f };
    int cost = 1;
};

struct GridCoord {
    int x, y;
};

class FlowMap {
public:
    double time = 0.0;
    int16_t refCount = 0;
    FlowCell* map;
    void* myWorldPtr;

    FlowMap(void* worldPtr, Vector2 targetGridPos);
};
