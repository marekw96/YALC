class SolidColor(YALCAnimation):
	color = [255,255,255]

	def name(self):
		return "SolidColor"

	def periodic(self, timeElapsedInUs):
		self._fill(self.color)

	def _fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])

	def getParameters(self):
		return [ColorParameter("fillColor", self.color)]

	def setParameter(self, name, value):
		if name == "fillColor":
			self.color = value


def create():
	return SolidColor()