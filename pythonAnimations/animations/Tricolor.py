class Tricolor(YALCAnimation):
	timeElapsed = int(0)
	counter = 0
	colors = [[255,0,0], [0,255,0], [0,0,255]]
	color_id = 0

	def name(self):
		return "Tricolor"

	def periodic(self, timeElapsedInUs):
		if self.timeElapsed > 1000000:
			self.timeElapsed = 0
			for i in range(0, self.getNumberOfPixels()):
				self.setPixelColor(i, self.colors[self.color_id][0], self.colors[self.color_id][1], self.colors[self.color_id][2])
				self.color_id = (self.color_id + 1) % 3

		self.timeElapsed = self.timeElapsed + int(timeElapsedInUs)


def create():
	return Tricolor()