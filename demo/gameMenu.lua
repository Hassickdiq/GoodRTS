local menuSize = 400
local SelectMap = ""

--Maps Menu | Window
function MapsWinThink(self, dt)
	self.states_closed = AllClose
end

--Maps Menu | Map List
function MapListFirstTime(self, dt) 
	local mapsDir = GetDir("maps")
	for i = 01, #mapsDir do
		self.logic_strvalues_push = mapsDir[i]
	end
end
function MapListClick(self, click, dt)
    if click == 1 then
		SelectMap = self.visual_string
		print("select ".. SelectMap)
    end
end

--Maps Menu | New Game Button
function NewGameBtnFirstTime(self, dt) 
	self.visual_string = "New Game"
end
function NewGameBtnClick(self, click, dt)
    if click == 1 then
		print(SelectMap)
		AllClose = true
		LoadMap("maps/".. SelectMap)
    end
end

--Main menu | Window
function MainMenuWinThink(self, dt)
	self.states_closed = AllClose
end

--Main menu | Title
function MainMenuTitleFirstTime(self, dt) 
	self.visual_string = "GoodRTS"
	self.visual_color = {0, 0, 0, 255}
	self.visual_str_size = 16
	self.transform_origin_x = menuSize / 2 - 35
end

--Main menu | Play Button
function MainBtn0FirstTime(self, dt) 
	self.visual_string = "Play"
end
function MainBtn0Click(self, click, dt)
    if click == 1 then
        local mapWinId = MakeWidget("WINDOW", 1280/2 - 200, 720/2 - 150, 400, 300, "MapsWin", "Maps")
        MakeWidget("LIST", 20, 40, 360, 180, "MapList", mapWinId)
        MakeWidget("BUTTON", 140, 235, 120, 35, "NewGameBtn", mapWinId)
    end
end

function MainBtn1FirstTime(self, dt) 
	self.visual_string = "Editor"
end
function MainBtn2FirstTime(self, dt) 
	self.visual_string = "Mods"
end
function MainBtn3FirstTime(self, dt) 
	self.visual_string = "Help"
end
function MainBtn4FirstTime(self, dt) 
	self.visual_string = "Exit"
end

function InitGameMenu()
	AllClose = false
	local window = MakeWidget("WINDOW", 1280/2 - menuSize/2, 720/2 - 250, menuSize, menuSize*1.2 , "MainMenuWin", "Main Menu")
	MakeWidget("TEXT", menuSize/2, 35, 0, 0, "MainMenuTitle", window)
	for i = 0, 4 do
		local prefix = "MainBtn".. i
		MakeWidget("BUTTON", menuSize/6, 90 + i * 45, menuSize/1.5, 40, prefix, window)
	end
end

