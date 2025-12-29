class Sparks(YALCAnimation):
    timeElapsed = int(0)
    counter = int(0)
    color = [255,255,255]
    fadeProps = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1]
    ids = []


    def __init__(self):
        self.initialise()

    def rand(self, seed):
        return int((seed * 23) + 12345) % 17

    def fade(self):
        for id in range(self.getNumberOfPixels()):
            red = self.color[0] * 0.1
            green = self.color[1] * 0.1
            blue = self.color[2] * 0.1
            self.setPixelColor(id, int(red), int(green), int(blue))

    def initialise(self):
        self.fade()

    def name(self):
        return "Sparks"

    def periodic(self, timeElapsedInUs):
        id = 0
        randomNum = self.rand(timeElapsedInUs % 17)
        if self.counter == 0:
            self.fade()
            self.ids = []
            while id < self.getNumberOfPixels():
                id += randomNum
                if id < self.getNumberOfPixels():
                    self.ids.append(id)
                randomNum = self.rand(randomNum)

        if self.timeElapsed > 100000:
            self.timeElapsed = 0
            self.counter = (self.counter + 1) % 20
            for idx in self.ids:
                fadeQ = self.fadeProps[self.counter]
                red = int(self.color[0] * fadeQ)
                green = int(self.color[1] * fadeQ)
                blue = int(self.color[2] * fadeQ)
                self.setPixelColor(idx, int(red), int(green), int(blue))

        self.timeElapsed = self.timeElapsed + int(timeElapsedInUs)

    def getParameters(self):
        return [ColorParameter("color", self.color)]

    def setParameter(self, name, value):
        if name == "color":
            self.color = value
        self.initialise()

def create():
    return Sparks()