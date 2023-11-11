class YALCAnimation:
	def name(self):
		return "Unnamed animation"

	def getNumberOfPixels(self):
		return __YALC__get_number_of_pixels()

	def periodic(self, timeElapsedInMs):
		pass

	def setPixelColor(self, id, r,g,b):
        __YALC__set_pixel_color_rgb(id, r,g,b)