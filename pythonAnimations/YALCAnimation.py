import leds

class ColorParameter:
	def __init__(self, name, defaultColor):
		self.name = name
		self.value = '{:02x}{:02x}{:02x}'.format(defaultColor[0],defaultColor[1], defaultColor[2])

	def register(self):
		leds.register_color_parameter(self.name, self.value)

class Input:
	INVALID = 0
	DIGITAL = 1
	ANALOG = 2
	RAW = 3

	def __init__(self, name, type):
		self.name = name
		self.type = type

	def register(self):
		leds.register_input(self.name, self.type)

class YALCAnimation:
	def name(self):
		return "Unnamed animation"

	def getNumberOfPixels(self):
		return leds.get_number_of_pixels()

	def periodic(self, timeElapsedInUs):
		pass

	def setPixelColor(self, id, r,g,b):
		leds.set_pixel_id_r_g_b(id, r,g,b)

	def getParameters(self):
		return []

	def setParameter(self, name, value):
		pass

	def registerParameters(self):
		for param in self.getParameters():
			param.register()

	def getInputs(self):
		return []

	def registerInputs(self):
		for input in self.getInputs():
			input.register()

	def handleInput(self, name, value):
		pass