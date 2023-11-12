class MyAnimation(YALCAnimation):
	timeElapsed = 0
	counter = 0
	colors = [[255,0,0], [0,255,0], [0,0,255]]
	color_id = 0

	def name(self):
		return "MyAnimation"

	def periodic(self, timeElapsedInMs):
		if self.timeElapsed > 100:
			self.timeElapsed = 0
			self.setPixelColor(self.counter, self.colors[self.color_id][0], self.colors[self.color_id][1], self.colors[self.color_id][2])
			self.counter = self.counter + 1

		self.timeElapsed = self.timeElapsed + timeElapsedInMs

		if self.counter > self.getNumberOfPixels():
		    self.counter = 0
		    self.color_id = (self.color_id + 1) % len(self.colors)


def create():
	return MyAnimation()