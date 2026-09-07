function TREESpawn(self, globaltime, dt)
	self.states_dontMove = true
	self.states_canItTouch = true
	self.states_canItSee = false
	PlayAnimation(self.id, 0, math.random(0, 7))
end