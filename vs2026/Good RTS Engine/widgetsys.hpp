#pragma once
#include "main.hpp"
#include "chacesys.hpp"
#include "Iosys.hpp"

enum wForm {
	RECTANGLE,
	CIRCLE,
	TEXT,
	WINDOW,
	BUTTON,
	TEXTBOX,
	LIST,
	CHECKBOX,
	SLIDER,
	FORM_COUNT, // non
};

struct wTransform
{
	Vector2 origin = { 0, 0 };
	Vector2  scale = { 0, 0 };
	int16_t size = 0;
	float angle = 0;
};

struct wBoundbox {
	Vector2 min;
	Vector2 max;
};

struct wVisual
{
	Anim2D animation;
	Color color = WHITE;
	std::string string = "";
	u8 str_size = 12;
};

struct wAuido
{
	std::vector<Sound*> sounds = { nullptr };
	float volume = 0;
	float pitch = 1.0f;
};

struct wStates {
	bool is_touch_mouse = false;
	bool is_active = true;
	bool is_firsttime = true;
	bool dont_think = false;
	bool closed = false;
};

struct wLogic
{	
	wForm form = FORM_COUNT;
	double nexttime = 0.1;
	std::string luaPrefix = "";
	int16_t owner = -1;
	std::vector<size_t>* dynamic_vector = nullptr;
	std::vector<std::string>* str_values = nullptr;
	std::vector<int>* order_list = nullptr;
};

class Widget
{	
public:
	int16_t id = -1;
	wTransform transform;
	wBoundbox boundbox;
	wVisual visual;
	wAuido audio;
	wLogic logic;
	wStates states;

	void FirstTime(lua_State* L, int id, float dt);
	void Think(lua_State* L, int id, double globaltime, float dt);
	void Click(lua_State* L, int id, uint8_t lastClick, float dt);
	void Touch(lua_State* L, int id, float dt);
	void Leave(lua_State* L, int id, float dt);
	void OnEvent(lua_State* L, int id, int activator, int type, const std::vector<size_t>& payload, float dt);

	void Update(void* myGUIptr, int16_t orderIndex, bool transmission, double globaltime);
};
