function ARROWSpawn(self, globaltime, dt)
	local angles = self.transform_angles
	local directionIndex = math.modf((angles + 22.5) / 45.0) % 8
	self.transform_direction = directionIndex
	self.states_dontMove = true
	self.states_canItTouch = true
	self.states_interactElev = false
	self.states_isNoticed = false
	self.states_canItSee = false
	self.states_isCollison = false
	self[0] = globaltime + 1.6 --lifetime
	
	if (directionIndex == 2) then
		PlayAnimation(self.id, 0, 0);

	elseif (directionIndex == 5) then
		PlayAnimation(self.id, 0, 3);

	elseif (directionIndex == 3) then
		PlayAnimation(self.id, 0, 1);

	elseif (directionIndex == 4) then
		PlayAnimation(self.id, 0, 2);

	elseif (directionIndex == 6) then
		PlayAnimation(self.id, 0, 4);

	elseif (directionIndex == 0) then
		PlayAnimation(self.id, 0, 2);

	elseif (directionIndex == 7) then
		PlayAnimation(self.id, 0, 3);

	elseif (directionIndex == 1) then
		PlayAnimation(self.id, 0, 1);
	end
end

function ARROWTouch(self, touchEntity, dt)
	if self[1] ~= touchEntity.id and touchEntity.ownerID ~= self.ownerID then
		if self.eClass ~= touchEntity.eClass then
			touchEntity.logic_hitPoint = touchEntity.logic_hitPoint - self[2]
			self.logic_hitPoint = 0
		end
	end
end

function ARROWThink(self, gt, dt)
	local rads = math.rad(self.transform_angles)
	local dirx = math.cos(rads)
	local diry = math.sin(rads)
	
	self.transform_origin_x = self.transform_origin_x + dirx * 450 * dt
	self.transform_origin_y = self.transform_origin_y + diry * 450 * dt
	
	if self[0] < gt then
		self.logic_hitPoint = 0
	end
 	
	self.logic_nexttime = gt
end