TIMER = 2500000
TIME_TO_LED = 10000

CLOSED = 0
OPENING = 1
DONE = 2
CLOSING = 3

class SensorLedMiddle(YALCAnimation):
	color = [10, 10, 10]
	status = CLOSED
	enabled = False
	timeToClose = 0
	lastLedTimeElapsed = 0
	lastPixel = 0

	def name(self):
		return "SensorLedMiddle"

	def periodic(self, timeElapsedInUs):
		self._fill([0,0,0])

		if self.enabled:
			self.timeToClose = TIMER

		if self.status == DONE:
			if self.timeToClose < 0:
				self.status = CLOSING
			else:
				self.timeToClose -= timeElapsedInUs

		elif self.status == OPENING:
			self.lastLedTimeElapsed += timeElapsedInUs
			if self.lastLedTimeElapsed >= TIME_TO_LED:
				how_many_leds = self.lastLedTimeElapsed // TIME_TO_LED
				self.lastPixel += how_many_leds
				self.lastLedTimeElapsed = self.lastLedTimeElapsed - (TIME_TO_LED * how_many_leds)

			if self.lastPixel >= self.getNumberOfPixels()//2:
				self.status = DONE
				self.lastLedTimeElapsed = 0

		elif self.status == CLOSING:
			if self.enabled:
				self.status = OPENING
			else:
				self.lastLedTimeElapsed += timeElapsedInUs
				if self.lastLedTimeElapsed >= TIME_TO_LED:
					how_many_leds = self.lastLedTimeElapsed // TIME_TO_LED
					self.lastPixel -= how_many_leds
					self.lastLedTimeElapsed = self.lastLedTimeElapsed - (TIME_TO_LED * how_many_leds)

				if self.lastPixel <= 0:
					self.status = CLOSED

		elif self.status == CLOSED:
			if self.enabled:
				self.status = OPENING

		self._fillFromMiddle(self.color, self.lastPixel)

	def _fillFromMiddle(self, color, size):
		middleLed = self.getNumberOfPixels()//2

		for i in range(size):
			self.setPixelColor(middleLed + i, color[0], color[1], color[2])
			self.setPixelColor(middleLed - i, color[0], color[1], color[2])

	def _fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])

	def getInputs(self):
		return [
			Input("PIR", Input.DIGITAL),
		]

	def getParameters(self):
		return [ColorParameter("fillColor", self.color)]

	def handleInput(self, name, value):
		self.enabled = True if value == 1 else False

	def setParameter(self, name, value):
		self.color = value


def create():
	return SensorLedMiddle()