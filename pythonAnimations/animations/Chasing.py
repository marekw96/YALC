PIXELS_PER_ITEM = 33

class Chasing(YALCAnimation):
	timeElapsed = int(0)
	counter = 0
	colors = []
	ids = []

	def __init__(self):
		self.initialise()

	def initialise(self):
		items = self.getNumberOfPixels() //PIXELS_PER_ITEM
		print("items", items)

		for i in range(items):
			self.colors.append([255,255,255])
			self.ids.append(i*33)

		self._fill([0,0,0])

	def name(self):
		return "Chasing"

	def periodic(self, timeElapsedInUs):
		if self.timeElapsed > 3300*17:
			self.timeElapsed = 0
			self._fill([0,0,0])
			id = 0
			for color in self.colors:
				self.setInBound(self.ids[id], color)
				self.setInBound(self.ids[id]-1, self.fade(color, 0.9))
				self.setInBound(self.ids[id]-2, self.fade(color, 0.6))
				self.setInBound(self.ids[id]-3, self.fade(color, 0.4))
				self.setInBound(self.ids[id]-4, self.fade(color, 0.2))

				self.ids[id] += 1
				if(self.ids[id] > self.getNumberOfPixels()+4):
					self.ids[id] = 0

				id += 1

		self.timeElapsed += timeElapsedInUs


	def fade(self, color, percent):
		return [int(color[0] * percent), int(color[1] * percent), int(color[2] * percent)]

	def _fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])

	def setInBound(self, id, color):
		if id >= 0 and id < self.getNumberOfPixels():
			self.setPixelColor(id, color[0], color[1], color[2])

	def getParameters(self):
		args = []
		id = 0
		for color in self.colors:
			args.append(ColorParameter("color_{}".format(id), color))
			id += 1
		return args

	def setParameter(self, name, value):
		id = int(name.split("color_")[1])
		self.colors[id] = value


def create():
	return Chasing()