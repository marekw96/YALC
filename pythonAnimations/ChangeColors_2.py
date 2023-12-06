STEPS = 500

class ChangeColors2(YALCAnimation):
    color_1 = [255,255,0]
    color_2 = [0,0,0]
    current_color = [255,255,0]
    step = [1,1,1]
    counter = 0
    timeCounter = 0

    def __init__(self):
        self.initialise()

    def initialise(self):
        self.step[0] = (self.color_2[0] - self.color_1[0])/STEPS
        self.step[1] = (self.color_2[1] - self.color_1[1])/STEPS
        self.step[2] = (self.color_2[2] - self.color_1[2])/STEPS
        self.counter = 0
        self.current_color[0] = self.color_1[0]
        self.current_color[1] = self.color_1[1]
        self.current_color[2] = self.color_1[2]
        self._fill(self.current_color)

    def name(self):
        return "ChangeColors2"

    def periodic(self, timeElapsedInUs):
        if self.timeCounter > 3300:
            self.counter += 1
            self.timeCounter = 0
            if self.counter >= STEPS-1:
                self.counter = 0
                self.step[0] *= -1
                self.step[1] *= -1
                self.step[2] *= -1
            self.current_color[0] += self.step[0]
            self.current_color[1] += self.step[1]
            self.current_color[2] += self.step[2]
            self._fill([int(self.current_color[0]),int(self.current_color[1]),int(self.current_color[2])])


        self.timeCounter += timeElapsedInUs

    def _fill(self, color):
        for i in range(self.getNumberOfPixels()):
            self.setPixelColor(i, color[0], color[1], color[2])

    def getParameters(self):
        return [ColorParameter("color_1", self.color_1), ColorParameter("color_2", self.color_2)]

    def setParameter(self, name, value):
        if name == "color_1":
            self.color_1 = value
        elif name == "color_2":
            self.color_2 = value

        self.initialise()


def create():
    return ChangeColors2()