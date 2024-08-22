class InputExample(YALCAnimation):
	color = [255,255,255]

	def name(self):
		return "InputExample"

	def periodic(self, timeElapsedInUs):
		self._fill(self.color)

	def _fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])

	def getInputs(self):
		return [
			Input("input_1", Input.RAW),
		]

	def handleInput(self, name, value):
		self.color = [value, 0, 0]


def create():
	return InputExample()