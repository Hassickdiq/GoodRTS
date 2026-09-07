function WATCHTOWERSpawn(self, globaltime, dt)
	self.states_dontMove = true
	self.states_canItTouch = true
	self.states_interactElev = false
end

function WATCHTOWERSaw(self, distance, target, dt)
	if target.ownerID ~= self.ownerID then
		if self.nowAction == 0 then
			GiveOrder(self.id, "ATTACK", -1, target.id)
		end
	end
end

function WATCHTOWERAttack(self, target, gt, dt)
	local diffX = target.transform_origin_x - self.transform_origin_x
	local diffY = target.transform_origin_y - self.transform_origin_y
	local distance = math.sqrt((diffX * diffX) + (diffY * diffY))
	
	local dirX = diffX / distance
	local dirY = diffY / distance	
	
	local rads = math.atan(diffY, diffX)
	local degrees = math.deg(rads)
	
	if degrees < 0 then 
		degrees = degrees + 360.0	
	end
	
	PlaySound(self.id, math.random(1, 2));
	local ent = MakeEntity("Arrow", self.transform_origin_x, self.transform_origin_y, self.ownerID)
	ent.transform_angles = degrees
	ent[1] = self.id
	ent[2] = math.random(self.logic_minDamage, self.logic_maxDamage)
end