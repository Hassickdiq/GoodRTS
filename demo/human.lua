function HUMANSpawn(self, globaltime, dt)
	self[0] = 0  --tx
	self[1] = 0  --ty
	self[2] = -1 --anim
	self[3] = 0  --frame
	
	self[4] = 0  --itemType
	self[5] = 0  --stack
	self[6] = 0  --shortwork
	self[7] = -1 --target id
	self.logic_moveSpeed = 60.0
	self.states_interactElev = true
	self.states_canItTouch = true
end

function HUMANAnimationManager(self)
	local anim = math.tointeger(self[2])
	
	if anim == 0 then
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 14 > self[3]) then
				PlayAnimation(self.id, anim, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 14 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then   -- left up and right up
			if not (self[3] > 14 and 29 > self[3]) then
				PlayAnimation(self.id, anim, 14)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 29 then
				PlayAnimation(self.id, anim, 14)
			end
			
		elseif self.transform_direction == 4 or self.transform_direction == 0 then  	-- left down and right down
			if not (self[3] > 29 and 44 > self[3]) then
				PlayAnimation(self.id, anim, 29)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 44 then
				PlayAnimation(self.id, anim, 29)
			end
		
		elseif self.transform_direction == 5 or self.transform_direction == 7 then 	-- left and right
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
		
	elseif anim == 1 then
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 10 > self[3]) then
				PlayAnimation(self.id, anim, 0)
				PlaySound(self.id, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 10 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then   -- left up and right up
			if not (self[3] > 10 and 21 > self[3]) then
				PlayAnimation(self.id, anim, 10)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 21 then
				PlayAnimation(self.id, anim, 10)
			end
			
		elseif self.transform_direction == 4 or self.transform_direction == 0 then  	-- left down and right down
			if not (self[3] > 21 and 32 > self[3]) then
				PlayAnimation(self.id, anim, 21)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] >32 then
				PlayAnimation(self.id, anim, 21)
			end
		
		elseif self.transform_direction == 5 or self.transform_direction == 7 then 	-- left and right
			if not (self[3] > 32 and 43 > self[3]) then
				PlayAnimation(self.id, anim, 32)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 43 then
				PlayAnimation(self.id, anim, 32)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 43 and 54 > self[3]) then
				PlayAnimation(self.id, anim, 43)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 54 then
				PlayAnimation(self.id, anim, 43)
			end
		end
	elseif anim == 2 then
		if self.transform_direction == 2 then 		-- down
			if not (self[3] > 0 and 26 > self[3]) then
				PlayAnimation(self.id, anim, 0)
				PlaySound(self.id, 0)
			end
			
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 26 then
				PlayAnimation(self.id, anim, 0)
			end
			
		elseif self.transform_direction == 3 or self.transform_direction == 1 then   -- left up and right up
			if not (self[3] > 26 and 52 > self[3]) then
				PlayAnimation(self.id, anim, 26)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 52 then
				PlayAnimation(self.id, anim, 26)
			end
			
		elseif self.transform_direction == 4 or self.transform_direction == 0 then  	-- left down and right down
			if not (self[3] > 56 and 82 > self[3]) then
				PlayAnimation(self.id, anim, 56)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 82 then
				PlayAnimation(self.id, anim, 56)
			end
		
		elseif self.transform_direction == 5 or self.transform_direction == 7 then 	-- left and right
			if not (self[3] > 82 and 108 > self[3]) then
				PlayAnimation(self.id, anim, 82)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 108 then
				PlayAnimation(self.id, anim, 82)
			end
			
		elseif self.transform_direction == 6 then 	-- up
			if not (self[3] > 108 and 134 > self[3]) then
				PlayAnimation(self.id, anim, 108)
				PlaySound(self.id, 0)
			end
				
			self[3] = PlayAnimation(self.id, anim)
			if self[3] > 134 then
				PlayAnimation(self.id, anim, 108)
			end
		end
	end
end

function HUMANMove(self, target, dt)
	self[2] = 0
	self[4] = 0
end

function HUMANStop(self, target, dt)
	self[2] = -1
	if self[4] == 1 or self[4] == 2 then
		self[6] = 1
		self[7] = -1
	end
end

function HUMANThink(self, gt, dt)
	HUMANAnimationManager(self)
end

function HUMANTouch(self, touchEntity, dt)
	if touchEntity.eClass == 247 then
		if self[5] > 0 then
			players[self.ownerID][0] = players[localPlr][0] + math.tointeger(self[5])
			self[5] = 0
			if self[7] ~= -1 then
				GiveOrder(self.id, "ATTACK", -1, self[7])
			end
		end
		
	elseif touchEntity.eClass == 244 then
		if self[5] > 0 then
			players[self.ownerID][1] = players[localPlr][1] + math.tointeger(self[5])
			self[5] = 0
			if self[7] ~= -1 then
				GiveOrder(self.id, "ATTACK", -1, self[7])
			end
		end
		
	end
	
end

function HUMANSaw(self, distance, target, dt)
	if self[5] == 0 and self[4] == 1 and target.eClass == 248 and math.random(0, 10) == 5 then
		if self[7] == -1 then
			self[7] = target.id
			GiveOrder(self.id, "ATTACK", -1, target.id)
		end
	end
	
	if self[5] == 0 and self[4] == 2 and target.eClass == 245 and math.random(0, 10) == 5 then
		if self[7] == -1 then
			self[7] = target.id
			GiveOrder(self.id, "ATTACK", -1, target.id)
		end
	end

	if target.eClass == 247 then --storeroom
		if (self[5] >= 15 or self[6] == 1) and self[4] == 1 then
			GiveOrder(self.id, "ATTACK", -1, target.id)
			self[6] = 0
		end
	end
	
	if target.eClass == 244 then --granary
		if (self[5] >= 15 or self[6] == 1) and self[4] == 2 then
			GiveOrder(self.id, "ATTACK", -1, target.id)
			self[6] = 0
		end
	end
end

function HUMANAttack(self, target, gt, dt)
	self[2] = -1
	if target.eClass == 248 then --tree
		self[2] = 1
		if self[4] ~= 1 then
			self[4] = 1
			self[5] = 0
		end
		
		target.logic_hitPoint = target.logic_hitPoint - 5
		self[5] = self[5] + 1
		self[7] = target.id
		
	elseif target.eClass == 245 then --fruit
		self[2] = 2
		if self[4] ~= 2 then
			self[4] = 2
			self[5] = 0
		end
		
		target.logic_hitPoint = target.logic_hitPoint - 5
		self[5] = self[5] + 1
		self[7] = target.id
	end
end

function HUMANFollow(self, target, dt)
	self[2] = 0
end