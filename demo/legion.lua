function LEGIONSpawn(self, globaltime, dt)
	self[0] = 0 --tx
	self[1] = 0 --ty
	self[2] = -1 --anim
	self[3] = 0
	self.states_interactElev = true
end

function LEGIONAnimationManager(self)
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

function LEGIONMove(self, target, dt)
	self[2] = 0
end

function LEGIONStop(self, target, dt)
	self[2] = -1
end

function LEGIONThink(self, gt, dt)
	LEGIONAnimationManager(self)
	
	if self.nowAction == 0 then
		self.states_canItSee = true
	else	
		self.states_canItSee = false
	end
end

function LEGIONSaw(self, distance, target, dt)
	if target.ownerID ~= self.ownerID and target.ownerID ~= 9 and self.nowAction == 0 and math.random(0, 2) == 0 then
		GiveOrder(self.id, "ATTACK", -1, target.id)
	end
end

function LEGIONAttack(self, target, gt, dt)
	target.logic_hitPoint = target.logic_hitPoint - math.random(self.logic_minDamage, self.logic_maxDamage)
	PlaySound(self.id, math.random(0, 2));
	self[2] = 1
end

function LEGIONFollow(self, target, dt)
	self[2] = 0
end