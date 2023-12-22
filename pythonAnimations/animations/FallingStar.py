class FallingStar(YALCAnimation):
    timeElapsed = int(0)
    counter = 0
    colors = [[64,0,0], [255,0,0], [220,0,0], [150,0,0], [128,0,0], [96,0,0], [64,0,0], [32,0,0], [16,0,0]]

    def name(self):
        return "FallingStar"

    def periodic(self, timeElapsedInUs):
        if self.timeElapsed > 50000:
            self._fill([0,0,0])
            for id in range(len(self.colors)):
                self.setInBound(self.counter - id, self.colors[id])

            self.counter += 1
            self.timeElapsed = 0

        self.timeElapsed = self.timeElapsed + timeElapsedInUs

        if self.counter > (self.getNumberOfPixels() + len(self.colors)):
            self.counter = 0

    def _fill(self, color):
        for i in range(self.getNumberOfPixels()):
            self.setPixelColor(i, color[0], color[1], color[2])

    def setInBound(self, id, color):
        if id >= 0 and id < self.getNumberOfPixels():
            self.setPixelColor(id, color[0], color[1], color[2])


def create():
	return FallingStar()