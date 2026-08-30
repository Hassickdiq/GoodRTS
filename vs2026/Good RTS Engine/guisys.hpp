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