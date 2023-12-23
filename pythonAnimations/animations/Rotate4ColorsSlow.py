STATE_SOLID = 1
STATE_CHANGE = 2

STATE_SOLID_WAIT = 100000000#10 seconds
STEPS = 600

def calculateStep(color1, color2, steps):
	return [(int(color2[0]) - int(color1[0])) / steps,
		 	(int(color2[1]) - int(color1[1])) / steps,
			(int(color2[2]) - int(color1[2])) / steps]

class Rotate4ColorsSlow(YALCAnimation):
	colors = [[255,255,255], [255,0,0], [0,255,0], [0,0,255]]
	steps = [[0,0,0] for i in range(4)]

	def __init__(self):
		self.initialise()

	def initialise(self):
		self.timeElapsed = 0
		self.stepCounter = 0
		self.state = STATE_SOLID
		self.currentColors = [[self.colors[0][0],self.colors[0][1], self.colors[0][2]],
							  [self.colors[1][0],self.colors[1][1], self.colors[1][2]],
							  [self.colors[2][0],self.colors[2][1], self.colors[2][2]],
							  [self.colors[3][0],self.colors[3][1], self.colors[3][2]]]
		self.offset = 0
		self.calculateSteps(self.offset)

	def calculateSteps(self, offset):
		self.steps[0] = calculateStep(self.colors[offset%4], self.colors[(offset+1)%4], STEPS)
		self.steps[1] = calculateStep(self.colors[(offset+1)%4], self.colors[(offset+2)%4], STEPS)
		self.steps[2] = calculateStep(self.colors[(offset+2)%4], self.colors[(offset+3)%4], STEPS)
		self.steps[3] = calculateStep(self.colors[(offset+3)%4], self.colors[(offset+4)%4], STEPS)

	def fillWith(self, colors):
		for pos in range(self.getNumberOfPixels()):
			color = colors[pos%4]
			self.setPixelColor(pos, int(color[0]), int(color[1]), int(color[2]))

	def name(self):
		return "Rotate4ColorsSlow"

	def periodic(self, timeElapsedInUs):
		if self.state == STATE_SOLID:
			self.stepCounter = 0
			if self.timeElapsed > STATE_SOLID_WAIT:
				self.state = STATE_CHANGE
		else:
			if self.stepCounter >= STEPS-1:
				self.timeElapsed = 0
				self.state = STATE_SOLID
				self.offset += 1
				self.calculateSteps(self.offset)
			else:
				self.stepCounter += 1
				for colorId in range(4):
					for rgb in range(3):
						self.currentColors[colorId][rgb] += self.steps[colorId][rgb]

		self.fillWith(self.currentColors)
		self.timeElapsed += timeElapsedInUs

	def setInBound(self, id, color):
		if id >= 0 and id < self.getNumberOfPixels():
			self.setPixelColor(id, color[0], color[1], color[2])

	def getParameters(self):
		return [ColorParameter("color0", self.colors[0]),
		  		ColorParameter("color1", self.colors[1]),
				ColorParameter("color2", self.colors[2]),
				ColorParameter("color3", self.colors[3])]

	def setParameter(self, name, value):
		id = int(name[-1])
		self.colors[id] = value

		self.initialise()


def create():
	return Rotate4ColorsSlow()