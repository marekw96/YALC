class Crazy(YALCAnimation):
    timeElapsed = int(0)
    changeTimer = int(0)
    timers = [ 100000, 1000000, 2000000, 1000000 ]
    timercounter = 0
    counter = 0
    colors = [[255,255,255], [255,0,0]]
    color_id = 0

    def __init__(self):
        self.initialise()

    def initialise(self):
        for id in range(self.getNumberOfPixels()):
            self.setPixelColor(id, self.colors[id%2][0], self.colors[id%2][1], self.colors[id%2][2])

    def name(self):
        return "Crazy"

    def periodic(self, timeElapsedInUs):

        if self.changeTimer > 20000000:
            self.changeTimer = 0
            self.timercounter = (self.timercounter + 1) % len(self.timers)

        if self.timeElapsed > self.timers[self.timercounter]:
            self.timeElapsed = 0
            for i in range(0, self.getNumberOfPixels()):
                if i % 2:
                    color = self.color_id
                    self.setPixelColor(i, self.colors[color][0], self.colors[color][1], self.colors[color][2])
                else:
                    color = (self.color_id +1 ) % 2
                    self.setPixelColor(i, self.colors[color][0], self.colors[color][1], self.colors[color][2])
            self.color_id = (self.color_id + 1) % 2

        self.timeElapsed = self.timeElapsed + int(timeElapsedInUs)
        self.changeTimer = self.changeTimer + int(timeElapsedInUs)

    def getParameters(self):
        return [ColorParameter("color_1", self.colors[0]), ColorParameter("color_2", self.colors[1])]

    def setParameter(self, name, value):
        if name == "color_1":
            self.colors[0] = value
        elif name == "color_2":
            self.colors[1] = value
        self.initialise()

def create():
    return Crazy()