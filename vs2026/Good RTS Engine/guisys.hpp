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
#include "widgetsys.hpp"
#include "Iosys.hpp"

class GameGUI {
public:
	std::vector<int> WidgetIndexStack;
	std::vector<int> WidgetOrder;

    bool UIInteraction = false;
    double globaltime = 0.0;
    lua_State* L = nullptr;
	Widget* WidgetList[WIDGET_LIMIT];
    GameCache Cache;
    GameIO gios;

    GameGUI(lua_State* mainL);
    void Delete();
    void Render(std::vector<int>* Order, bool transmission);

    void ResetWidgetVector();
    void PushWidget(Widget* widget);
    void PopWidget(int16_t id);
    Widget* MakeWidget(wForm form, Vector2 origin, Vector2 scale, std::string luaprefix);

    static int LuaGetDir(lua_State* L);
    static int LuaGetWidget(lua_State* L);
    static int LuaMakeWidget(lua_State* L);

    static int WidgetIndex(lua_State* L);
    static int WidgetNewIndex(lua_State* L);
};
