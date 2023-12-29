class Wave(YALCAnimation):
	timeElapsed = int(0)
	counter = 0
	colors = [[255,0,0], [0,255,0], [0,0,255]]
	color_id = 0

	def name(self):
		return "Wave"

	def periodic(self, timeElapsedInUs):
		if self.timeElapsed > 10000:
			self.timeElapsed = 0
			self.setPixelColor(50 - self.counter, self.colors[self.color_id][0], self.colors[self.color_id][1], self.colors[self.color_id][2])
			self.setPixelColor(50 + self.counter, self.colors[self.color_id][0], self.colors[self.color_id][1], self.colors[self.color_id][2])

			self.counter = self.counter + 1

		self.timeElapsed = self.timeElapsed + int(timeElapsedInUs)

		if self.counter > self.getNumberOfPixels() / 2:
			self.counter = 0
			self.color_id = (self.color_id + 1) % 3

def create():
	return Wave()