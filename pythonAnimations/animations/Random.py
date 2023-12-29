class Random(YALCAnimation):
    changeTimer = int(0)
    counter = 100
    color = [255,255,255]
    last_color = [0,0,0]
    led_list = []
    time = 125000
    phase = 1
    select = 10

    def __init__(self):
        self.initialise()

    def initialise(self):
        for id in range(self.getNumberOfPixels()):
            self.setPixelColor(id, self.color[0], self.color[1], self.color[2])
        self.shuffle_list(0)

    def name(self):
        return "Random"

    def periodic(self, timeElapsedInUs):
        seed = self.changeTimer
        if (self.changeTimer > self.time and self.phase == 0):
            self.time = self.time - 1250
            self.changeTimer = 0
            self.setPixelColor(self.led_list.pop(), self.color[0], self.color[1], self.color[2])
            self.counter = self.counter + 1

        if (self.changeTimer > 500000 and self.phase == 1):
            self.changeTimer = 0
            if (self.select % 2 == 0):
                for i in range(self.getNumberOfPixels()):
                    self.setPixelColor(i, self.last_color[0], self.last_color[1], self.last_color[2])
            else:
                for i in range(self.getNumberOfPixels()):
                    self.setPixelColor(i, self.color[0], self.color[1], self.color[2])
            self.select = self.select + 1

        self.changeTimer = self.changeTimer + int(timeElapsedInUs)

        if (self.counter >= self.getNumberOfPixels() and self.phase == 1 and self.select == 10):
            self.counter = 0
            self.draw_color()
            self.shuffle_list(seed)
            self.time = 125000
            self.phase = 0
            self.select = 0
        elif (self.counter >= self.getNumberOfPixels()):
            self.phase = 1

    def draw_color(self):
        seed = self.changeTimer % 255
        self.last_color[0] = self.color[0]
        self.last_color[1] = self.color[1]
        self.last_color[2] = self.color[2]
        self.color[0] = (seed) % 255
        self.color[1] = (self.color[0] + seed * seed + 3) % 255
        self.color[2] = (seed * seed + 3) % 255


    def shuffle_list(self, pseed):
        self.led_list = []
        temp_list = []
        seed = pseed % self.getNumberOfPixels()

        for i in range(self.getNumberOfPixels()):
            temp_list.append(i)

        while len(temp_list) > 0:
            seed = (seed * seed + 3) % len(temp_list)
            self.led_list.append(temp_list.pop(seed))

def create():
    return Random()