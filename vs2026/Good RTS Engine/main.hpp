#pragma once
#define SCREEN_HEIGHT 720
#define SCREEN_WIDTH 1280
#define FPS_LIMIT 0
#define TILE_WIDTH 64
#define TILE_HEIGHT 32
#define ENTITY_LIMIT 2048
#define WIDGET_LIMIT 512
#define EVENT_LIMIT 255
#define SELECT_LIMIT 30
#define E_DEBUG false

#include <iostream>
#include <cstdint>
#include <unordered_map>
#include "lsrc/lua.hpp"
#include <vector>
#include <string>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define i32 int32_t
#define u64 uint64_t

struct vec2 {
	i32 x;
	i32 y;
};