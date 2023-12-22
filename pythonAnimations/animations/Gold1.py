class Gold1(YALCAnimation):
    timeElapsed = int(0)
    counter = 0
    counter_1 = 33
    counter_2 = 66
    colors = [[150, 150, 0],
              [200, 200, 0],
              [255, 255, 0],
              [240, 240, 0],
              [200, 200, 0],
              [170, 170, 0],
              [150, 150, 0]]

    def name(self):
        return "Gold1"

    def periodic(self, timeElapsedInUs):
        if self.timeElapsed > 50000:
            self._fill([110,110,0])
            for id in range(len(self.colors)):
                self.setInBound(self.counter - id, self.colors[id])
                self.setInBound(self.counter_1 - id, self.colors[id])
                self.setInBound(self.counter_2 - id, self.colors[id])

            self.counter += 1
            self.counter_1 += 1
            self.counter_2 += 1
            self.timeElapsed = 0

        self.timeElapsed = self.timeElapsed + timeElapsedInUs

        if self.counter > (self.getNumberOfPixels() + len(self.colors)):
            self.counter = 0
        if self.counter_1 > (self.getNumberOfPixels() + len(self.colors)):
            self.counter_1 = 0
        if self.counter_2 > (self.getNumberOfPixels() + len(self.colors)):
            self.counter_2 = 0

    def _fill(self, color):
        for i in range(self.getNumberOfPixels()):
            self.setPixelColor(i, color[0], color[1], color[2])

    def setInBound(self, id, color):
        if id >= 0 and id < self.getNumberOfPixels():
            self.setPixelColor(id, color[0], color[1], color[2])


def create():
    return Gold1()