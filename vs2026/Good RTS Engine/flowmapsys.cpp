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

#include "flowmapsys.hpp"
#include "worldsys.hpp"

using namespace std;

FlowMap::FlowMap(void* worldPtr, Vector2 targetGridPos) {
	myWorldPtr = worldPtr;
	GameWorld* myWorld = (GameWorld*)myWorldPtr;

	u32 width = myWorld->map->width;
	u32 height = myWorld->map->height;

    map = new FlowCell[width * height];

    if (!map) return;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            map[idx].cost = 1;
            map[idx].direction = { 0, 0 };
        }
    }

    for (int i = 0; i < ENTITY_LIMIT; i++) {
        Entity* ent = myWorld->EntityList[i];
        if (ent && ent->states.isAnchor) {

            Vector2 gridPos = WorldToGrid(ent->transform.origin);
            int gx = (int)(gridPos.x + 0.5f);
            int gy = (int)(gridPos.y + 0.5f);

            if (gx >= 0 && gx < width && gy >= 0 && gy < height) {
                map[gy * width + gx].cost = 255;
            }
        }
    }

    std::vector<int> integrationValues(width * height, 9999);
    std::queue<GridCoord> q;

    int targetX = (int)targetGridPos.x;
    int targetY = (int)targetGridPos.y;

    if (targetX < 0)
        targetX = 0;

    if (targetX >= (int)width)
        targetX = (int)width;

    if (targetY < 0)
        targetY = 0;    

    if (targetY >= (int)height)
        targetY = (int)height;

    int targetIdx = targetY * width + targetX;
    integrationValues[targetIdx] = 0;
    q.push({ targetX, targetY });

    int dx[] = { 0, 0, 1, -1 };
    int dy[] = { 1, -1, 0, 0 };

    while (!q.empty()) {
        GridCoord current = q.front();
        q.pop();

        int currIdx = current.y * width + current.x;

        for (int i = 0; i < 4; i++) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                int nextIdx = ny * width + nx;

                if (map[nextIdx].cost != 255) {
                    if (integrationValues[currIdx] + map[nextIdx].cost < integrationValues[nextIdx]) {
                        integrationValues[nextIdx] = integrationValues[currIdx] + map[nextIdx].cost;
                        q.push({ nx, ny });
                    }
                }
            }
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = y * width + x;
            if (map[idx].cost == 255) continue;

            int bestX = x;
            int bestY = y;
            int lowestValue = integrationValues[idx];

            for (int dy_offset = -1; dy_offset <= 1; dy_offset++) {
                for (int dx_offset = -1; dx_offset <= 1; dx_offset++) {
                    if (dx_offset == 0 && dy_offset == 0) continue;

                    int nx = x + dx_offset;
                    int ny = y + dy_offset;

                    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                        int nIdx = ny * width + nx;
                        if (integrationValues[nIdx] < lowestValue) {
                            lowestValue = integrationValues[nIdx];
                            bestX = nx;
                            bestY = ny;
                        }
                    }
                }
            }

            float dirX = (float)(bestX - x);
            float dirY = (float)(bestY - y);

            float length = sqrt(dirX * dirX + dirY * dirY);
            if (length > 0) {
                map[idx].direction = { dirX / length, dirY / length };
            }
        }
    }
}
