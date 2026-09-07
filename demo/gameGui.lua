local AllClose = false
players = {}
localPlr = -1
skillBox = {}
skillSetClear = true

function CloseWorld()
	AllClose = true
end

--Unit HitPoint Box
function UnitHitPointBGFirstTime(self, dt) 
	self.visual_color = {0, 0, 0, 255}
end
function UnitHitPointFGFirstTime(self, dt) 
	self.visual_color = {10, 160, 0, 255}
end
function UnitHitPointFGThink(self, gt, dt) 
	self.visual_color = {10, 160, 0, 255}
	entList = GetPlayerState(localPlr, "selectedEnt");

	if entList[0] ~= -1 then
		local ent = GetEntity(entList[0])
		if ent.isThere == false then 
			self.transform_scale_x = 0
			return 
		end	
		
		local part = 50.0 / ent.logic_maxHitPoint
		self.transform_scale_x = part * ent.logic_hitPoint
	end
	
	self.logic_nexttime = gt + 0.3
end
--Unit Damages Box
function UnitDamagesFirstTime(self, dt) 
	self.visual_color = {140, 0, 0, 255}
	self.visual_strOffX = 5.0
	self.visual_strOffY = 5.0
	self.visual_aligType = 1
end
function UnitDamagesThink(self, gt, dt) 
	entList = GetPlayerState(localPlr, "selectedEnt");

	if entList[0] ~= -1 then
		local ent = GetEntity(entList[0])
		if ent.isThere == false then 
			self.visual_string = ""
			return 
		end	
		
		local text = "A: ".. ent.logic_maxDamage.. " - ".. ent.logic_minDamage
		
		self.visual_string = text
		self.transform_scale_x = #text*5 + 15
	end
	
	self.logic_nexttime = gt + 0.3
end
--Unit Icon Box
function UnitPnlIconFirstTime(self, dt) 
	self.visual_animation = "UnitIcon"
	self.visual_animIndex = 1
	
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.9
	
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
end
function UnitPnlIconThink(self, dt) 
	if localPlr ~= -1 then
		entList = GetPlayerState(localPlr, "selectedEnt");
		
		if entList[0] ~= -1 then
			local ent = GetEntity(entList[0])
			if ent.isThere == false then 
				self.visual_viewImage = false
				return 
			end	
			
			if ent.eClass == 248 then 	  --Tree
				self.visual_animIndex = 35
			elseif ent.eClass == 254 then --Legion
				self.visual_animIndex = 16
			elseif ent.eClass == 249 then --Human
				self.visual_animIndex = 0	
				--Skill Set
				skillBox[1].visual_animIndex = 0
				skillBox[1].visual_viewImage = true
				
				skillBox[4].visual_animIndex = 2
				skillBox[4].visual_viewImage = true
				
				skillBox[7].visual_animIndex = 3
				skillBox[7][0] = 1
				skillBox[7].visual_viewImage = true
			elseif ent.eClass == 255 then --Ballista
				self.visual_animIndex = 9
			elseif ent.eClass == 253 then --Archer Rider
				self.visual_animIndex = 29
			elseif ent.eClass == 245 then --Fruit
				self.visual_animIndex = 36
			end		
			
			self.visual_viewImage = true
			skillSetClear = false
		else
			self.visual_viewImage = false
			
			if skillSetClear == false then
				for i = 1, 12 do 
					skillBox[i].visual_viewImage = false
				end
				skillSetClear = true
			end
		end
	end
end

--Orders
function UseSkill(self, player)
	skill = self[0]
	
	if skill == 1 then --Stop
		GiveOrder(self.id, "NOP", player)
		
	end
end

--Control Panel Window
function ControlPnlThink(self, dt)
	self.states_closed = AllClose
end
--Skill Boxs
function Slot0FirstTime(self, dt) 
	self.visual_string = "Q"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[1] = self
end
function Slot1FirstTime(self, dt)
	self.visual_string = "A"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[2] = self
end
function Slot2FirstTime(self, dt)
	self.visual_string = "Z"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[3] = self
end
function Slot3FirstTime(self, dt)
	self.visual_string = "W"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[4] = self
end
function Slot4FirstTime(self, dt)
	self.visual_string = "S"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[5] = self
end
function Slot5FirstTime(self, dt)
	self.visual_string = "X"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[6] = self
end
function Slot6FirstTime(self, dt)
	self.visual_string = "E"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[7] = self
end
function Slot6Click(self, lastClick, dt)
	UseSkill(self, localPlr)
end
function Slot7FirstTime(self, dt)
	self.visual_string = "D"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[8] = self
end
function Slot7Click(self, lastClick, dt)
	UseSkill(self, localPlr)
end
function Slot8FirstTime(self, dt)
	self.visual_string = "C"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[9] = self
end
function Slot9FirstTime(self, dt)
	self.visual_string = "R"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[10] = self
end
function Slot10FirstTime(self, dt)
	self.visual_string = "F"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[11] = self
end
function Slot11FirstTime(self, dt)	
	self.visual_string = "V"
	self.visual_animation = "OrderIcon"
	--image setting
	self.visual_viewImage = false
	self.visual_imageColor = {255, 255, 255, 255}
	self.visual_imageSize = 0.7
	--image offset
	self.visual_imageOffX = 2.0
	self.visual_imageOffY = 2.0
	--string offset
	self.visual_strOffX = -12.0
	self.visual_strOffY = -6.0
	skillBox[12] = self
end

--Source Panel Window
function SourcePnlThink(self, dt)
	self.states_closed = AllClose
end
--Wood Counter
function WoodCounterFirstTime(self, dt)	
	self.visual_color = {161, 124, 100, 255}
	self.visual_string = "".. players[localPlr][0]
end
function WoodCounterThink(self, dt)	
	self.visual_string = "".. players[localPlr][0]
end
--Food Counter
function FoodCounterFirstTime(self, dt)	
	self.visual_color = {184, 99, 99, 255}
	self.visual_string = "".. players[localPlr][1]
end
function FoodCounterThink(self, dt)	
	self.visual_string = "".. players[localPlr][1]
end
--Menu Button
function MenuBtnFirstTime(self, dt)	
	self.visual_color = {60, 60, 60, 255}
	self.visual_string = "Menu"
end
function MenuBtnClick()
	InitGameMenu()
end

--Start GUI
function InitWorld(_localPlr)
	AllClose = false
	localPlr = _localPlr
	for i = 0, 8 do
		players[i] = {}
		players[i][0] = 0 -- 0: Wood
		players[i][1] = 0 -- 1: Food
		players[i][2] = 0 -- 2: Gold
	end

	local ControlPnl = MakeWidget("WINDOW", 0, 540, 1280, 180, "ControlPnl", "Control Panel")
	MakeWidget("BUTTON", 8, 25, 50, 50, "UnitPnlIcon", ControlPnl)
	MakeWidget("BUTTON", 8, 85, 50, 10, "UnitHitPointBG", ControlPnl)
	MakeWidget("BUTTON", 8, 85, 50, 10, "UnitHitPointFG", ControlPnl)
	MakeWidget("BUTTON", 8, 105, 0, 20, "UnitDamages", ControlPnl)
	for i = 0, 11 do
		local prefix = "Slot".. i
		MakeWidget("BUTTON", 120 + (i / 3) *45, 30 + (i % 3) * 45, 40, 40, prefix, ControlPnl)
	end
	
	local SourcePnl = MakeWidget("WINDOW", 0, 0, 1280, 40, "SourcePnl", "Source Panel")
	MakeWidget("BUTTON", 0 , 20, 50, 20, "WoodCounter", SourcePnl)
	MakeWidget("BUTTON", 50 , 20, 50, 20, "FoodCounter", SourcePnl)
	MakeWidget("BUTTON", 1220 , 20, 60, 20, "MenuBtn", SourcePnl)
end