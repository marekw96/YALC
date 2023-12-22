class Pixels3Static(YALCAnimation):
	colors = [[255,0,0], [0,255,0], [0,0,255]]

	def __init__(self):
		self.initialise()

	def initialise(self):
		for id in range(self.getNumberOfPixels()):
			self.setPixelColor(id, self.colors[id%3][0], self.colors[id%3][1], self.colors[id%3][2])

	def name(self):
		return "Pixels3Static"

	def periodic(self, timeElapsedInUs):
		pass

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
	return Pixels3Static()