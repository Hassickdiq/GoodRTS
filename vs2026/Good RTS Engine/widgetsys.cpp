#include "widgetsys.hpp"
#include "guisys.hpp"

using namespace std;

// FirstTime
void Widget::FirstTime(lua_State* L, int id, float dt) {
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "FirstTime").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

// Think
void Widget::Think(lua_State* L, int id, double globaltime, float dt) {
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "Think").c_str());
	
	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, globaltime);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

void Widget::Touch(lua_State* L, int id, float dt) {
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "Touch").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

void Widget::Leave(lua_State* L, int id, float dt) {
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "Leave").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 2, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

void Widget::Click(lua_State* L, int id, uint8_t lastClick, float dt) {
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "Click").c_str());

	if (lua_isfunction(L, -1)) {
		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushinteger(L, lastClick);

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 3, 0, 0) != LUA_OK) {
			std::cout << "Script Calisma Hatasi: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

void Widget::OnEvent(lua_State* L, int id, int activator, int type, const std::vector<size_t>& payload, float dt){
	if (!this) return;

	lua_getglobal(L, (logic.luaPrefix + "OnEvent").c_str());

	if (lua_isfunction(L, -1)) {

		lua_newtable(L);
		lua_pushinteger(L, id);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_newtable(L);
		lua_pushinteger(L, activator);
		lua_setfield(L, -2, "id");
		luaL_getmetatable(L, "WidgetMeta");
		lua_setmetatable(L, -2);

		lua_pushinteger(L, type);

		lua_newtable(L);
		for (size_t i = 0; i < payload.size(); ++i) {
			lua_pushinteger(L, payload[i]);
			lua_rawseti(L, -2, i + 1); // tablo[i+1] = payload[i]
		}

		lua_pushinteger(L, dt);

		if (lua_pcall(L, 5, 0, 0) != LUA_OK) {
			std::cout << "Hata: " << lua_tostring(L, -1) << std::endl;
			lua_pop(L, 1);
		}
	}
	else {
		lua_pop(L, 1);
	}
}

void Widget::Update(void* myGUIptr, int16_t orderIndex, bool transmission, double globaltime) {
	float dt = GetFrameTime();
	if (dt > 0.05f) dt = 0.05f;

	// GUI
	GameGUI* myGUI = (GameGUI*)(myGUIptr);
	lua_State* L = myGUI->L;
	GameIO* gios = &myGUI->gios;
	Widget** WidgetList = myGUI->WidgetList;
	vector<int>* WidgetOrder = &myGUI->WidgetOrder;

	// this
	Vector2 origin = transform.origin;
	Vector2 scale  = transform.scale;
	int size       = transform.size;
	std::vector<size_t>* dv = logic.dynamic_vector;

	if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
		boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {
		myGUI->UIInteraction = true;
	}

	if (logic.form == SLIDER) {
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);
			if (dv->size() < 3) {
				dv->resize(3, 0);
			}
			else {
				(*dv)[0] = 0;
				(*dv)[1] = 0;
				(*dv)[2] = 0;
			}

			if (WidgetList[logic.owner])
				WidgetList[logic.owner]->logic.order_list->push_back(id);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.01;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission)
		{	
			Widget* owner = nullptr;
			int owner_id = logic.owner;

			Vector2 o_origin = { 0, 0 };

			if (WidgetList[owner_id]) {
				owner = WidgetList[owner_id];
				o_origin = owner->transform.origin;
			}

			if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
				boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {
				if (IsMouseButtonDown(0) && (*dv)[0] == 0) {
					(*dv)[0] = 1;
					(*dv)[1] = (size_t)((*dv)[2] - gios->MousePos.y);
				}
			}

			if (!IsMouseButtonDown(0) && (*dv)[0] == 1) {
				(*dv)[0] = 0;
			}
			else {
				if ((*dv)[0] == 1)
					(*dv)[2] = (size_t)(gios->MousePos.y + (int)(*dv)[1]);

				if ((int)(*dv)[2] > scale.y)
					(*dv)[2] = scale.y;

				if ((int)(*dv)[2] < 0)
					(*dv)[2] = 0;
			}
		}
		return;
	}

	if (logic.form == CHECKBOX) {
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);
			dv->push_back(0);

			if (WidgetList[logic.owner])
				WidgetList[logic.owner]->logic.order_list->push_back(id);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.01;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission)
		{
			if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
				boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {

				if (!states.is_touch_mouse)
				{
					states.is_touch_mouse = true;
					Touch(L, id, dt);
				}
				if (gios->lastClick == 1) {
					Click(L, id, gios->lastClick, dt);

					if (gios->lastClick == 1 && (*dv)[0] == 0)
						(*dv)[0] = 1;
					else {
						if (gios->lastClick == 1 && (*dv)[0] == 1)
							(*dv)[0] = 0;
					}
					gios->lastClick = 0;
				}
			}
			else {
				if (states.is_touch_mouse)
				{
					states.is_touch_mouse = false;
					Leave(L, id, dt);
				}
			}
		}

		return;
	}

	if (logic.form == LIST) {
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);
			dv->push_back(0);
			dv->push_back(0);

			if (WidgetList[logic.owner])
				WidgetList[logic.owner]->logic.order_list->push_back(id);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.01;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission){
			if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
				boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {

				if (gios->lastClick == 1) {
					int index = static_cast<int>((gios->MousePos.y - boundbox.min.y) / visual.str_size);
					(*dv)[1] = index;

					if (index < logic.str_values->size())
						visual.string = (*logic.str_values)[index];

					Click(L, id, gios->lastClick, dt);
					gios->lastClick = 0;
				}
			}
		}
		return;
	}

	if (logic.form == TEXTBOX) {
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);
			dv->push_back(0);

			if (WidgetList[logic.owner])
				WidgetList[logic.owner]->logic.order_list->push_back(id);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.01;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission)
		{
			if (gios->focus == id) {
				if (gios->backspace){
					if (!visual.string.empty())
						visual.string.pop_back();
					gios->backspace = 0;
				}
				else if (gios->enter) {
					if (!visual.string.empty())
						OnEvent(myGUI->L, id, id, 257, {}, dt);
					gios->enter = 0;
				}

				if (gios->lastKey != 0){
					visual.string += gios->lastKey;
					gios->lastKey = 0;
				}
			}

			if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
				boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {

				if (!states.is_touch_mouse)
				{
					states.is_touch_mouse = true;
					Touch(L, id, dt);
				}
				if (gios->lastClick == 1) {
					Click(L, id, gios->lastClick, dt);
					gios->focus = id;
					gios->lastClick = 0;
				}
			}
			else {
				if (states.is_touch_mouse){
					states.is_touch_mouse = false;
					Leave(L, id, dt);
				}
			}
		}
		return;
	}

	if (logic.form == BUTTON) {
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);

			if (WidgetList[logic.owner])
				WidgetList[logic.owner]->logic.order_list->push_back(id);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.01;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission)
		{

			if (boundbox.max.x > gios->MousePos.x && boundbox.min.x < gios->MousePos.x &&
				boundbox.max.y > gios->MousePos.y && boundbox.min.y < gios->MousePos.y) {

				if (!states.is_touch_mouse)
				{
					states.is_touch_mouse = true;
					Touch(L, id, dt);
				}
				if (gios->lastClick == 1) {
					Click(L, id, gios->lastClick, dt);
					gios->lastClick = 0;
				}
			}
			else {
				if (states.is_touch_mouse)
				{
					states.is_touch_mouse = false;
					Leave(L, id, dt);
				}
			}
		}
		return;
	}

	if (logic.form == WINDOW){
		if (states.is_firsttime) {
			states.is_firsttime = false;
			FirstTime(L, id, dt);
			dv->push_back(0);
			dv->push_back(0);
			dv->push_back(0);
			dv->push_back(0);
		}

		if (logic.nexttime < globaltime) {
			logic.nexttime = globaltime + 0.1;
			if (!states.dont_think)
				Think(L, id, globaltime, dt);
		}

		if (transmission)
		{
			// Move
			if (origin.x < gios->MousePos.x && gios->MousePos.x < (origin.x + scale.x) &&
				boundbox.min.y < gios->MousePos.y && gios->MousePos.y < origin.y + 20) {
				if (IsMouseButtonDown(0) && (*dv)[0] == 0) {
					(*dv)[0] = 1;
					(*dv)[1] = (size_t)(origin.x - gios->MousePos.x);
					(*dv)[2] = (size_t)(origin.y - gios->MousePos.y);
					auto it = std::find(WidgetOrder->begin(), WidgetOrder->end(), id);
					if (it != WidgetOrder->end() && it != WidgetOrder->end() - 1) {
						int val = *it;
						WidgetOrder->erase(it);
						WidgetOrder->push_back(val);
					}
				}
			}
			if (!IsMouseButtonDown(0) && (*dv)[0] == 1) {
				(*dv)[0] = 0;
			}
			else {
				if ((*dv)[0] == 1)
				{
					origin.x = gios->MousePos.x + (int)(*dv)[1];
					origin.y = gios->MousePos.y + (int)(*dv)[2];
				}
			}

			// Resize
			if (origin.x + scale.x - 20 < gios->MousePos.x && gios->MousePos.x < (origin.x + scale.x) &&
				boundbox.min.y + scale.y - 20 < gios->MousePos.y && gios->MousePos.y < origin.y + scale.y) {
				if (IsMouseButtonDown(0) && (*dv)[3] == 0) {
					(*dv)[3] = 1;
					auto it = std::find(WidgetOrder->begin(), WidgetOrder->end(), id);
					if (it != WidgetOrder->end() && it != WidgetOrder->end() - 1) {
						int val = *it;
						WidgetOrder->erase(it);
						WidgetOrder->push_back(val);
					}
				}
			}

			// Close
			if (origin.x + scale.x - 20 < gios->MousePos.x && gios->MousePos.x < (origin.x + scale.x) &&
				origin.y + 20 > gios->MousePos.y && gios->MousePos.y > origin.y) {
				if (gios->lastClick == 1){
					states.closed = true;
					return;
				}
			}

			if (!IsMouseButtonDown(0) && (*dv)[3] == 1) {
				(*dv)[3] = 0;
			}
			else {
				if ((*dv)[3] == 1)
				{
					scale.x = (origin.x - gios->MousePos.x - 20) * -1;
					scale.y = (origin.y - gios->MousePos.y - 20) * -1;

					if (scale.x < 100)
						scale.x = 100;

					if (scale.y < 100)
						scale.y = 100;
				}
			}
			transform.origin = origin;
			transform.scale  = scale;
			return;
		}
		return;
	}
}
