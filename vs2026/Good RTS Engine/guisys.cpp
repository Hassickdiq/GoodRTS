#include "guisys.hpp"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

GameGUI::GameGUI(lua_State* mainL) {
	ResetWidgetVector();

	L = mainL;

	luaL_newmetatable(L, "WidgetMeta");

	lua_pushlightuserdata(L, this);
	lua_setfield(L, -2, "__gui");

	lua_pushcfunction(L, WidgetIndex);
	lua_setfield(L, -2, "__index");

	lua_pushcfunction(L, WidgetNewIndex);
	lua_setfield(L, -2, "__newindex");

	lua_pushlightuserdata(L, this);
	lua_setfield(L, LUA_REGISTRYINDEX, "MyGameGUIInstance");

	lua_pushcfunction(L, GameGUI::LuaMakeWidget);
	lua_setglobal(L, "MakeWidget");

	lua_pushcfunction(L, GameGUI::LuaGetDir);
	lua_setglobal(L, "GetDir");

	lua_pushcfunction(L, GameGUI::LuaGetWidget);
	lua_setglobal(L, "GetWidget");
}

Widget* GameGUI::MakeWidget(wForm form, Vector2 origin, Vector2 scale, string luaprefix) {
	Widget* BasicWidget = new Widget();
	BasicWidget->visual.color = GRAY;
	BasicWidget->logic.form = form;
	BasicWidget->logic.luaPrefix = luaprefix;
	BasicWidget->transform.origin = origin;
	BasicWidget->transform.scale = scale;
	BasicWidget->transform.size = 1;
	BasicWidget->states.is_firsttime = true;
	BasicWidget->states.dont_think = false;
	BasicWidget->logic.order_list = new std::vector<int>();
	BasicWidget->logic.dynamic_vector = new std::vector<size_t>();
	return BasicWidget;
}

int GameGUI::LuaGetWidget(lua_State* L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "MyGameGUIInstance");
	GameGUI* myGUI = (GameGUI*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	if (lua_isinteger(L, 1)) {
		int16_t id = lua_tointeger(L, 1);

		if (myGUI->WidgetList[id]) {
			lua_newtable(L);
			lua_pushinteger(L, id);
			lua_setfield(L, -2, "id");
			luaL_getmetatable(L, "EntityMeta");
			lua_setmetatable(L, -2);

			return 1;
		}
	}
	return 0;
}

int GameGUI::LuaGetDir(lua_State* L) {
	if (lua_isstring(L, 1)){
		lua_newtable(L);

		std::string path = lua_tostring(L, 1);
		int index = 1;

		if (fs::exists(path) && fs::is_directory(path)) {
			for (const auto& entry : fs::directory_iterator(path)) {
				if (entry.path().extension() == ".MAP" || entry.path().extension() == ".map") {
					lua_pushstring(L, entry.path().filename().string().c_str());
					lua_rawseti(L, -2, index);

					index++;
				}
			}
		}
		return 1;
	}

	return 0;
}

int GameGUI::LuaMakeWidget(lua_State* L) {
	lua_getfield(L, LUA_REGISTRYINDEX, "MyGameGUIInstance");
	GameGUI* myGUI = (GameGUI*)lua_touserdata(L, -1);
	lua_pop(L, 1);

	if (lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isstring(L, 6)) {
		string form = lua_tostring(L, 1);
		Vector2 origin = { lua_tonumber(L, 2), lua_tonumber(L, 3) };
		Vector2 scale = { lua_tonumber(L, 4), lua_tonumber(L, 5) };
		string luaprefix = lua_tostring(L, 6);

		if (form == "WINDOW" && lua_isstring(L, 7))
		{	
			string title = lua_tostring(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(WINDOW, origin, scale, luaprefix);
			BasicWidget->visual.string = title;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		if (form == "BUTTON" && lua_isinteger(L, 7))
		{
			int16_t owner = lua_tointeger(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(BUTTON, origin, scale, luaprefix);
			BasicWidget->logic.owner = owner;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		if (form == "SLIDER" && lua_isinteger(L, 7))
		{
			int16_t owner = lua_tointeger(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(SLIDER, origin, scale, luaprefix);
			BasicWidget->logic.owner = owner;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		if (form == "TEXTBOX" && lua_isinteger(L, 7))
		{
			int16_t owner = lua_tointeger(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(TEXTBOX, origin, scale, luaprefix);
			BasicWidget->logic.owner = owner;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		if (form == "LIST" && lua_isinteger(L, 7))
		{
			int16_t owner = lua_tointeger(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(LIST, origin, scale, luaprefix);
			BasicWidget->logic.str_values = new vector<string>;
			BasicWidget->logic.owner = owner;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		if (form == "CHECKBOX" && lua_isinteger(L, 7))
		{
			int16_t owner = lua_tointeger(L, 7);
			Widget* BasicWidget = myGUI->MakeWidget(CHECKBOX, origin, scale, luaprefix);
			BasicWidget->logic.owner = owner;

			myGUI->PushWidget(BasicWidget);
			lua_pushinteger(L, BasicWidget->id);
			return 1;
		}
		else throw std::out_of_range("Invalid Form Template");
	}
	return 0;
}

void GameGUI::Delete() {
	Cache.UnloadAll();

	for (u16 i = 0; i < WIDGET_LIMIT; i++) {
		Widget* wid = WidgetList[i];
		if (wid) {
			if (wid->logic.order_list)
				delete wid->logic.order_list;
			if (wid->logic.dynamic_vector)
				delete wid->logic.dynamic_vector;
			if (wid->logic.str_values)
				delete wid->logic.str_values;

			delete WidgetList[i];
			WidgetList[i] = nullptr;
		}
	}
}

void GameGUI::ResetWidgetVector() {
	WidgetIndexStack.clear();
	WidgetIndexStack.reserve(WIDGET_LIMIT);
	for (size_t i = (size_t)WIDGET_LIMIT; i != 0; i--) {
		WidgetIndexStack.push_back(i - 1);
	}
}

void GameGUI::PushWidget(Widget* widget) {
	if (WidgetIndexStack.empty()) {
		throw std::runtime_error("There is not enough space!");
	}

	int index = WidgetIndexStack.back();
	WidgetIndexStack.pop_back();
	widget->id = index;
	WidgetList[index] = widget;
	WidgetOrder.push_back(index);
}

void GameGUI::PopWidget(int16_t index) {
	if (index < 0 || index >= ENTITY_LIMIT) {
		throw std::out_of_range("Invalid index!");
	}

	if (WidgetList[index]) {
		if (WidgetList[index]->logic.order_list)
			if (WidgetList[index]->logic.order_list){
				for (int wid : *WidgetList[index]->logic.order_list) {
					PopWidget(wid);
				}
				delete WidgetList[index]->logic.order_list;
			}

		if (WidgetList[index]->logic.dynamic_vector)
			delete WidgetList[index]->logic.dynamic_vector;
		if (WidgetList[index]->logic.str_values)
			delete WidgetList[index]->logic.str_values;

		delete(WidgetList[index]);
		WidgetList[index] = nullptr;
		WidgetIndexStack.push_back(index);
	}
}

void GameGUI::Render(std::vector<int>* Order, bool transmission) {
	if (!Order) return;

	for (size_t ix = Order->size()-1; ix != -1; ix--) {
		int i = (*Order)[ix];

		if (WidgetList[i]) {
			Widget* widget = WidgetList[i];
			Widget* owner  = WidgetList[widget->logic.owner];

			if (widget->states.closed){
				PopWidget(widget->id);
				return;
			}

			Vector2 owner_origin = {0, 0};
			if (owner)
				owner_origin = owner->transform.origin;

			// x
			widget->boundbox.max.x = widget->transform.origin.x + owner_origin.x + (widget->transform.scale.x * widget->transform.size);
			widget->boundbox.min.x = widget->transform.origin.x + owner_origin.x;

			// y
			widget->boundbox.max.y = widget->transform.origin.y + owner_origin.y + (widget->transform.scale.y * widget->transform.size);
			widget->boundbox.min.y = widget->transform.origin.y + owner_origin.y;

			if (true)
			{	
				widget->Update(this, ix, transmission, globaltime);

				if (widget->boundbox.max.x > gios.MousePos.x && widget->boundbox.min.x < gios.MousePos.x &&
					widget->boundbox.max.y > gios.MousePos.y && widget->boundbox.min.y < gios.MousePos.y)
				{	
					if (widget->logic.order_list){
						Render(widget->logic.order_list, transmission);
						transmission = false;
					}
				}
			}
		}
	}

	for (size_t ix = 0; ix < Order->size(); ix++) {
		int i = (*Order)[ix];
		Widget* widget = WidgetList[i];

		if (widget) {
			wForm   form			= widget->logic.form;
			Color   color		    = widget->visual.color;
			Vector2 origin			= widget->transform.origin;
			Vector2 scale			= widget->transform.scale;
			int size				= widget->transform.size;
			std::vector<size_t>* dv = widget->logic.dynamic_vector;

			Vector2 o_origin = { 0, 0 };

			if (WidgetList[widget->logic.owner])
				o_origin = WidgetList[widget->logic.owner]->transform.origin;

			switch (widget->logic.form)
			{
			case RECTANGLE: {
				DrawRectangle(origin.x, origin.y, scale.x * size, scale.y * size, color);
				break;
			}
			case CIRCLE: {
				DrawCircle(origin.x, origin.y, scale.x * size, color);
				break;
			}
			case TEXT: {
				const char* text = widget->visual.string.c_str();
				DrawText(text, origin.x, origin.y, scale.x * size, color);
				break;
			}
			case WINDOW: {
				DrawRectangle(origin.x, origin.y, scale.x, scale.y, GRAY);
				DrawRectangle(origin.x, origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x, origin.y, scale.x, 2, { 255, 255, 255, 128 });
				DrawRectangle((origin.x + scale.x - 2), origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x, (origin.y + scale.y - 2), scale.x, 2, { 0, 0, 0, 128 });

				scale.y = 20;

				DrawRectangle(origin.x, origin.y, scale.x, scale.y, LIGHTGRAY);
				DrawRectangle(origin.x, origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x, origin.y, scale.x, 2, { 255, 255, 255, 128 });
				DrawRectangle((origin.x + scale.x - 2), origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x, (origin.y + scale.y - 2), scale.x, 2, { 0, 0, 0, 128 });

				DrawText(widget->visual.string.c_str(), origin.x + 8, origin.y, 16, BLACK);

				origin.x += scale.x - 20;
				scale.x = 20;

				DrawRectangle(origin.x, origin.y, scale.x, scale.y, RED);
				DrawRectangle(origin.x, origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x, origin.y, scale.x, 2, { 255, 255, 255, 128 });
				DrawRectangle((origin.x + scale.x - 2), origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x, (origin.y + scale.y - 2), scale.x, 2, { 0, 0, 0, 128 });

				Render(widget->logic.order_list, transmission);
				transmission = false;
				break;
			}
			case BUTTON: {
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, scale.y, color);
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, 2, { 255, 255, 255, 128 });
				DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + scale.y - 2), scale.x, 2, { 0, 0, 0, 128 });

				int fSize = widget->visual.str_size;
				int sSize = widget->visual.string.size();

				if (sSize > 0){	
					int TxtSizeX = sSize * fSize;
					DrawText(widget->visual.string.c_str(), origin.x + o_origin.x + (scale.x / 2) - TxtSizeX/4, origin.y + o_origin.y + (scale.y / 4), widget->visual.str_size, WHITE);
				}
				break;
			}
			case TEXTBOX: {
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, scale.y, { (unsigned char)(color.r >> 1), (unsigned char)(color.g >> 1), (unsigned char)(color.b >> 1), color.a });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, 2, { 0, 0, 0, 128 });
				DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + scale.y - 2), scale.x, 2, { 255, 255, 255, 128 });
				if (widget->visual.string.size() > 0)
					DrawText(widget->visual.string.c_str(), origin.x + o_origin.x + 5, origin.y + o_origin.y + (scale.y / 4), widget->visual.str_size, WHITE);
				break;
			}
			case LIST: {
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, scale.y, { (unsigned char)(color.r >> 1), (unsigned char)(color.g >> 1), (unsigned char)(color.b >> 1), color.a });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, 2, { 0, 0, 0, 128 });
				DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + scale.y - 2), scale.x, 2, { 255, 255, 255, 128 });

				if (!widget->states.is_firsttime) {
					int i_y = 0;
					for (int i = (*dv)[0]; i < (int)widget->logic.str_values->size(); i++) {
						if ((*dv)[1] == i){
							DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size), scale.x, widget->visual.str_size, color);
							DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size), 2, widget->visual.str_size, { 255, 255, 255, 128 });
							DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size), scale.x, 2, { 255, 255, 255, 128 });
							DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size), 2, widget->visual.str_size, { 0, 0, 0, 128 });
							DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size) + widget->visual.str_size - 2), scale.x, 2, { 0, 0, 0, 128 });
						}

						DrawText((*widget->logic.str_values)[i].c_str(), origin.x + o_origin.x + 5, origin.y + o_origin.y + 5 + (i_y * widget->visual.str_size), widget->visual.str_size, WHITE);
						if (((i_y + 3) * widget->visual.str_size) > scale.y)
							break;
						i_y++;
					}
				}
				break;
			}
			case CHECKBOX: {
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, scale.y, { (unsigned char)(color.r >> 1), (unsigned char)(color.g >> 1), (unsigned char)(color.b >> 1), color.a });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, 2, scale.y, { 0, 0, 0, 128 });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y, scale.x, 2, { 0, 0, 0, 128 });
				DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y, 2, scale.y, { 255, 255, 255, 128 });
				DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + scale.y - 2), scale.x, 2, { 255, 255, 255, 128 });
				if (!widget->states.is_firsttime) {
					if ((*dv)[0] == 1)
						DrawRectangle(origin.x + o_origin.x + 5, origin.y + o_origin.y + 5, scale.x - 10, scale.y - 10, { 196, 179, 87, color.a });
				}
				break;
			}
			case SLIDER: {
				Vector2 o_origin = { 0, 0 };

				if (WidgetList[widget->logic.owner])
					o_origin = WidgetList[widget->logic.owner]->transform.origin;

				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y - 30, scale.x, scale.y + 20, { (unsigned char)(color.r >> 1), (unsigned char)(color.g >> 1), (unsigned char)(color.b >> 1), color.a });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y - 30, 2, scale.y + 20, { 0, 0, 0, 128 });
				DrawRectangle(origin.x + o_origin.x, origin.y + o_origin.y - 30, scale.x, 2, { 0, 0, 0, 128 });
				DrawRectangle((origin.x + o_origin.x + scale.x - 2), origin.y + o_origin.y - 30, 2, scale.y + 20, { 255, 255, 255, 128 });
				DrawRectangle(origin.x + o_origin.x, (origin.y + o_origin.y + scale.y - 2) - 10, scale.x, 2, { 255, 255, 255, 128 });

				if (!widget->states.is_firsttime){
					int thumb_x = origin.x + o_origin.x;
					int thumb_y = (*dv)[2] + o_origin.y + 30;

					DrawRectangle(thumb_x, thumb_y, scale.x, 20, color);

					DrawRectangle(thumb_x, thumb_y, 2, 20, { 255, 255, 255, 128 });
					DrawRectangle(thumb_x, thumb_y, scale.x, 2, { 255, 255, 255, 128 });

					DrawRectangle(thumb_x + scale.x - 2, thumb_y, 2, 20, { 0, 0, 0, 128 });
					DrawRectangle(thumb_x, thumb_y + 20 - 2, scale.x, 2, { 0, 0, 0, 128 });
				}
				break;
			}
			default:
				return;
			}
		}
	}
}

int GameGUI::WidgetIndex(lua_State* L) {
	luaL_getmetatable(L, "WidgetMeta");
	lua_getfield(L, -1, "__gui");
	GameGUI* myWorld = (GameGUI*)lua_touserdata(L, -1);
	lua_pop(L, 2);

	lua_getfield(L, 1, "id");
	int id = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	if (myWorld->WidgetList[id]) {
		Widget* widget = myWorld->WidgetList[id];

		if (lua_isnumber(L, 2)) {
			long long raw_index = lua_tointeger(L, 2);

			if (raw_index < 0) {
				return luaL_error(L, "Index cannot be negative!");
			}

			size_t index = (size_t)raw_index;

			auto* dyn_vec = widget->logic.dynamic_vector;
			if (!dyn_vec) return 0;

			std::vector<size_t>& vec = *dyn_vec;

			if (index < vec.size()) {
				lua_pushinteger(L, vec[index]);
			}
			return 1;
		}

		const char* key = lua_tostring(L, 2);

		size_t keycode = 0;
		if (key[0] == 0) return 0;

		for (size_t i = 0; i < strlen(key); i++) {
			keycode += ((size_t)key[i] * (i + (size_t)key[i]));
		}
		keycode *= (keycode & (keycode + strlen(key) / key[0]));

		switch (keycode)
		{
		case 446265625: { // id
			lua_pushinteger(L, id);
			return 1;
		}
		case 51174583524: { // state_is_firsttime
			lua_pushboolean(L, widget->states.is_firsttime);
			return 1;
		}
		case 38332940944: { // state_is_active
			lua_pushboolean(L, widget->states.is_active);
			return 1;
		}
		case 63960433216: { // state_is_touch_mouse
			lua_pushboolean(L, widget->states.is_touch_mouse);
			return 1;
		}
		case 51872795536: { // transform_origin_x
			lua_pushinteger(L, widget->transform.origin.x);
			return 1;
		}
		case 51990384196: { // transform_origin_y
			lua_pushinteger(L, widget->transform.origin.y);
			return 1;
		}
		case 44059269409: { // transform_scale_x
			lua_pushinteger(L, widget->transform.scale.x);
			return 1;
		}
		case 44167225600: { // transform_scale_y
			lua_pushinteger(L, widget->transform.scale.y);
			return 1;
		}
		case 31339266841: { // transform_size
			lua_pushnumber(L, widget->transform.size);
			return 1;
		}
		case 33426443241: { // transform_angle
			lua_pushnumber(L, widget->transform.angle);
			return 1;
		}
		case 21952867225: { // visual_color
			lua_pushinteger(L, widget->visual.color.r);
			lua_pushinteger(L, widget->visual.color.g);
			lua_pushinteger(L, widget->visual.color.b);
			lua_pushinteger(L, widget->visual.color.a);
			return 4;
		}
		case 18350766225: { // visual_form
			lua_pushinteger(L, widget->logic.form);
			return 1;
		}
		case 26834043721: { // visual_string
			lua_pushstring(L, widget->visual.string.c_str());
			return 1;
		}
		case 65137758841: { // loigc_strvalues_pop
			lua_pushinteger(L, widget->logic.str_values->size());
			return 1;
		}
		default:
			cout << "Unknown key | " << key << ": " << keycode << endl;
			break;
		}
	}
	return 0;
}

int GameGUI::WidgetNewIndex(lua_State* L) {
	luaL_getmetatable(L, "WidgetMeta");
	lua_getfield(L, -1, "__gui");
	GameGUI* myWorld = (GameGUI*)lua_touserdata(L, -1);
	lua_pop(L, 2);

	lua_getfield(L, 1, "id");
	int id = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	if (myWorld->WidgetList[id]) {
		Widget* widget = myWorld->WidgetList[id];

		if (lua_isnumber(L, 2)) {
			long long raw_index = lua_tointeger(L, 2);

			if (raw_index < 0) {
				return luaL_error(L, "Index cannot be negative!");
			}

			size_t index = (size_t)raw_index;
			size_t val = (size_t)lua_tonumber(L, 3);

			auto* dyn_vec = widget->logic.dynamic_vector;
			if (!dyn_vec) return 0;

			std::vector<size_t>& vec = *dyn_vec;

			if (index >= vec.size()) {
				vec.resize(index + 1, 0);
			}

			vec[index] = val;
			return 0;
		}

		const char* key = lua_tostring(L, 2);

		size_t keycode = 0;
		if (key[0] == 0) return 0;

		for (size_t i = 0; i < strlen(key); i++) {
			keycode += ((size_t)key[i] * (i + (size_t)key[i]));
		}
		keycode *= (keycode & (keycode + strlen(key) / key[0]));

		switch (keycode)
		{
		case 38332940944: { // state_is_active
			widget->states.is_active = lua_toboolean(L, 3);
			return 0;
		}
		case 51872795536: { // transform_origin_x
			widget->transform.origin.x = (int)lua_tointeger(L, 3);
			return 0;
		}
		case 51990384196: { // transform_origin_y
			widget->transform.origin.y = (int)lua_tointeger(L, 3);
			return 0;
		}
		case 44059269409: { // transform_scale_x
			widget->transform.scale.x = (int)lua_tointeger(L, 3);
			return 0;
		}
		case 44167225600: { // transform_scale_y
			widget->transform.scale.y = (int)lua_tointeger(L, 3);
			return 0;
		}
		case 31339266841: { // transform_size
			widget->transform.size = (int)lua_tointeger(L, 3);
			return 0;
		}
		case 33426443241: { // transform_angle
			widget->transform.angle = (float)lua_tonumber(L, 3);
			return 0;
		}
		case 21952867225: { // visual_color
			int r, g, b, a;

			lua_rawgeti(L, -1, 1);
			r = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, 2);
			g = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, 3);
			b = lua_tointeger(L, -1);
			lua_pop(L, 1);

			lua_rawgeti(L, -1, 4);
			a = lua_tointeger(L, -1);
			lua_pop(L, 1);

			widget->visual.color.r = (r & 0xFF);
			widget->visual.color.g = (g & 0xFF);
			widget->visual.color.b = (b & 0xFF);
			widget->visual.color.a = (a & 0xFF);
			return 0;
		}
		case 18350766225: { // visual_form
			if (FORM_COUNT > lua_tointeger(L, 3))
				widget->logic.form = static_cast<wForm>(lua_tointeger(L, 3));
			return 0;
		}
		case 26834043721: { // visual_string
			if (lua_tostring(L, 3)) {
				widget->visual.string = lua_tostring(L, 3);
			}
			return 0;
		}
		case 65663550001: { // logic_strvalues_push
			widget->logic.str_values->push_back(lua_tostring(L, 3));
			return 0;
		}
		case 58202527504: { // logic_strvalues_pop
			for (size_t i = 0; i < lua_tonumber(L, 3); i++)
				if (!widget->logic.str_values->empty())
					widget->logic.str_values->pop_back();
			return 0;
		}
		case 24785149489: { // states_closed
			widget->states.closed = lua_toboolean(L, 3);
			return 0;
		}
		default:
			cout << "Unknown key | " << key << ": " << keycode << endl;
			break;
		}
	}
}