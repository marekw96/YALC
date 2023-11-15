import leds

class YALCAnimation:
	def name(self):
		return "Unnamed animation"

	def getNumberOfPixels(self):
		return leds.get_number_of_pixels()

	def periodic(self, timeElapsedInUs):
		pass

	def setPixelColor(self, id, r,g,b):
        leds.set_pixel_id_r_g_b(id, r,g,b)