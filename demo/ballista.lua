function BALLISTASpawn(self, dt)
	self[0] = 0 --tx
	self[1] = 0 --ty
	self[2] = -1 --anim
	self[3] = 0 --frame
	self.states_interactElev = false
end

function AnimationManager(self)
	local anim = math.tointeger(self[2])
	
	if anim == 0 then	
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 3 > self[3]) then
				PlayAnimation(self.id, anim, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 3 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 5 or self.transform_direction == 7 then   -- left up and right up
			if not (self[3] > 12 and 15 > self[3]) then
				PlayAnimation(self.id, anim, 12)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 15 then
				PlayAnimation(self.id, anim, 12)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then  	-- left down and right down
			if not (self[3] > 4 and 8 > self[3]) then
				PlayAnimation(self.id, anim, 4)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 7 then
				PlayAnimation(self.id, anim, 4)
			end
		
		elseif self.transform_direction == 4 or self.transform_direction == 0 then 	-- left and right
			if not (self[3] > 8 and 11 > self[3]) then
				PlayAnimation(self.id, anim, 8)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 11 then
				PlayAnimation(self.id, anim, 8)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 16 and 19 > self[3]) then
				PlayAnimation(self.id, anim, 16)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 19 then
				PlayAnimation(self.id, anim, 16)
			end
			
		elseif self.transform_direction == 0 then 	-- left
			if not (self[3] > 16 and 19 > self[3]) then
				PlayAnimation(self.id, anim, 16)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 19 then
				PlayAnimation(self.id, anim, 16)
			end
		end
		
	elseif anim == 1 then
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 3 > self[3]) then
				PlayAnimation(self.id, anim, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 3 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 5 or self.transform_direction == 7 then   -- left up and right up
			if not (self[3] > 12 and 15 > self[3]) then
				PlayAnimation(self.id, anim, 12)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 15 then
				PlayAnimation(self.id, anim, 12)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then  	-- left down and right down
			if not (self[3] > 4 and 8 > self[3]) then
				PlayAnimation(self.id, anim, 4)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 7 then
				PlayAnimation(self.id, anim, 4)
			end
		
		elseif self.transform_direction == 4 or self.transform_direction == 0 then 	-- left and right
			if not (self[3] > 8 and 11 > self[3]) then
				PlayAnimation(self.id, anim, 8)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 11 then
				PlayAnimation(self.id, anim, 8)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 16 and 19 > self[3]) then
				PlayAnimation(self.id, anim, 16)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 19 then
				PlayAnimation(self.id, anim, 16)
			end
		end
	end
end

function BALLISTAThink(self, gt, dt)
	AnimationManager(self)
	
	if self.nowAction == 0 then
		self.states_canItSee = true
	else	
		self.states_canItSee = false
	end
end

function BALLISTAMove(self, dt, tx, ty)
	PlaySound(self.id, 0);
	self[2] = 0
	self[0] = tx
	self[1] = ty
end

function BALLISTAAttack(self, target, gt, dt)
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
	
	PlaySound(self.id, math.random(1, 3));
	local ent = MakeEntity("BallistaArrow", self.transform_origin_x, self.transform_origin_y, self.ownerID)
	ent.transform_angles = degrees
	ent[1] = self.id
	ent[2] = math.random(self.logic_minDamage, self.logic_maxDamage)
end

function BALLISTASaw(self, distance, target, dt)
	if target.ownerID ~= self.ownerID and target.ownerID ~= 9 then
		if self.nowAction == 0 then
			GiveOrder(self.id, "ATTACK", -1, target.id)
		end
	end
end

function BALLISTAFollow(self, target, dt)
	self[2] = 0
end

function BALLISTAAroundMe(self, target, dt)
	self[2] = -1
end

function BALLISTAStop(self, dt, tx, ty)
	self[2] = -1
end