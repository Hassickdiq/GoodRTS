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
