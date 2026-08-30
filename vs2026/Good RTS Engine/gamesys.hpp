#pragma once
#include "main.hpp"
#include "worldsys.hpp"
#include "chacesys.hpp"
#include "guisys.hpp"
#include "Iosys.hpp"

class Game {
public:
	double globaltime = 0.0;
	GameWorld* world = nullptr;
	GameGUI* gui = nullptr;
	GameIO gios;
	lua_State* L = nullptr;

	Game();
	static Game* Quit(Game* game);
	GameWorld* LoadWorld(std::string path);
	void InitGameMenu(lua_State* L);
	static int LuaLoadMap(lua_State* L);

	void GameLoop();
};