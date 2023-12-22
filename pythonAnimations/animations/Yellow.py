class Yellow(YALCAnimation):
    def name(self):
        return "Yellow"

    def periodic(self, timeElapsedInUs):
        self._fill([255,255,0])

    def _fill(self, color):
        for i in range(self.getNumberOfPixels()):
            self.setPixelColor(i, color[0], color[1], color[2])

def create():
    return Yellow()