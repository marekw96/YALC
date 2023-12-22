STEPS = 500

class ChangeColors3(YALCAnimation):
	colors = [[255,0,0], [0,255,0], [0,0,255]]
	current_color = [255,0,0]
	step = [1,1,1]
	counter = 0
	color_id = 0
	timeCounter = 0

	def __init__(self):
		self.initialise()

	def initialise(self):
		self.color_id = 1
		self.step = self.calculateStep(self.colors[1], self.colors[0])
		self.counter = 0
		self.current_color[0] = self.colors[0][0]
		self.current_color[1] = self.colors[0][1]
		self.current_color[2] = self.colors[0][2]
		self._fill(self.current_color)

	def calculateStep(self, color1, color2):
		return [(color1[0] - color2[0])/STEPS, (color1[1] - color2[1])/STEPS, (color1[2] - color2[2])/STEPS]

	def name(self):
		return "ChangeColors3"

	def periodic(self, timeElapsedInUs):
		if self.timeCounter > 9900:
			self.counter += 1
			self.timeCounter = 0
			if self.counter >= STEPS-1:
				self.counter = 0
				nextColorId = (self.color_id + 1) % 3
				self.current_color[0] = self.colors[self.color_id][0]
				self.current_color[1] = self.colors[self.color_id][1]
				self.current_color[2] = self.colors[self.color_id][2]
				self.step = self.calculateStep(self.colors[nextColorId], self.colors[self.color_id])
				self.color_id = nextColorId

			self.current_color[0] += self.step[0]
			self.current_color[1] += self.step[1]
			self.current_color[2] += self.step[2]
			self._fill([int(self.current_color[0]),int(self.current_color[1]),int(self.current_color[2])])


		self.timeCounter += timeElapsedInUs

	def _fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])

	def getParameters(self):
		return [ColorParameter("color_1", self.colors[0]), ColorParameter("color_2", self.colors[1]),ColorParameter("color_3", self.colors[2])]

	def setParameter(self, name, value):
		if name == "color_1":
			self.colors[0] = value
		elif name == "color_2":
			self.colors[1] = value
		elif name == "color_3":
			self.colors[2] = value
		self.initialise()


def create():
	return ChangeColors3()