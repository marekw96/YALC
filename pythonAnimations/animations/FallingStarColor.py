class FallingStarColor(YALCAnimation):
    timeElapsed = int(0)
    counter = 0
    color = [255,255,255]
    colors = []
    fadeProps = [0.2, 1, 0.9, 0.7, 0.5, 0.4, 0.2, 0.1]
    ids = []

    def __init__(self):
        for id in range(self.getNumberOfPixels()//15):
            self.ids.append(id*15)

    def name(self):
        return "FallingStarColor"

    def periodic(self, timeElapsedInUs):
        if self.timeElapsed > 100000:
            self.timeElapsed = 0
            self.fill([0,0,0])

            for z in range(len(self.ids)):
                pos = self.ids[z] - 1
                for fade in self.fadeProps:
                    #self.setInBound(pos, self.color)
                    #print(fade)
                    self.setInBound(pos, [int(self.color[0] * fade), int(self.color[1] * fade), int(self.color[2] * fade)])
                    pos += 1

                if self.ids[z] > self.getNumberOfPixels():
                    self.ids[z] = 0
                else:
                    self.ids[z] += 1

        self.timeElapsed += timeElapsedInUs


    def fill(self, color):
        for i in range(self.getNumberOfPixels()):
            self.setPixelColor(i, color[0], color[1], color[2])

    def setInBound(self, id, color):
        if id >= 0 and id < self.getNumberOfPixels():
            self.setPixelColor(id, color[0], color[1], color[2])

    def getParameters(self):
        return [ColorParameter("color", self.color)]

    def setParameter(self, name, value):
        if name == "color":
            self.color = value

        self.initColors()

def create():
	return FallingStarColor()