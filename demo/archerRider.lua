function ARCHERRIDERSpawn(self, globaltime, dt)
	self[0] = 0 --tx
	self[1] = 0 --ty
	self[2] = -1 --anim
	self[3] = 0 --frame
	self.logic_moveSpeed = 85.0
	self.states_interactElev = false
end

function ARCHERRIDERAnimationManager(self)
	local anim = math.tointeger(self[2])
	
	if anim == 0 then
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 9 > self[3]) then
				PlayAnimation(self.id, anim, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 9 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then   -- left up and right up
			if not (self[3] > 9 and 19 > self[3]) then
				PlayAnimation(self.id, anim, 9)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 19 then
				PlayAnimation(self.id, anim, 9)
			end
			
		elseif self.transform_direction == 4 or self.transform_direction == 0 then  	-- left down and right down
			if not (self[3] > 19 and 29 > self[3]) then
				PlayAnimation(self.id, anim, 19)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 29 then
				PlayAnimation(self.id, anim, 19)
			end
		
		elseif self.transform_direction == 5 or self.transform_direction == 7 then 	-- left and right
			if not (self[3] > 29 and 39 > self[3]) then
				PlayAnimation(self.id, anim, 29)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 39 then
				PlayAnimation(self.id, anim, 29)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 39 and 49 > self[3]) then
				PlayAnimation(self.id, anim, 39)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 49 then
				PlayAnimation(self.id, anim, 39)
			end
		end
	
	elseif anim == 1 then	
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 14 > self[3]) then
				PlayAnimation(self.id, anim, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 14 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 5 or self.transform_direction == 7 then   -- left up and right up
			if not (self[3] > 14 and 29 > self[3]) then
				PlayAnimation(self.id, anim, 14)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 29 then
				PlayAnimation(self.id, anim, 14)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then  	-- left down and right down
			if not (self[3] > 29 and 44 > self[3]) then
				PlayAnimation(self.id, anim, 29)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 44 then
				PlayAnimation(self.id, anim, 29)
			end
		
		elseif self.transform_direction == 4 or self.transform_direction == 0 then 	-- left and right
			if not (self[3] > 44 and 59 > self[3]) then
				PlayAnimation(self.id, anim, 44)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 59 then
				PlayAnimation(self.id, anim, 44)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 59 and 74 > self[3]) then
				PlayAnimation(self.id, anim, 59)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 74 then
				PlayAnimation(self.id, anim, 59)
			end
		end
	end
end

function ARCHERRIDERMove(self, target, dt)
	PlaySound(self.id, 0);
	self[2] = 0
end

function ARCHERRIDERStop(self, target, dt)
	self[2] = -1
end

function ARCHERRIDERThink(self, gt, dt)
	ARCHERRIDERAnimationManager(self)
	
	if self.nowAction == 0 then
		self.states_canItSee = true
	else	
		self.states_canItSee = false
	end
end

function ARCHERRIDERSaw(self, distance, target, dt)
	if target.ownerID ~= self.ownerID and target.ownerID ~= 9 then
		if self.nowAction == 0 then
			GiveOrder(self.id, "ATTACK", -1, target.id)
		end
	end
end

function ARCHERRIDERAttack(self, target, gt, dt)
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

function ARCHERRIDERFollow(self, target, dt)
	self[2] = 0
end