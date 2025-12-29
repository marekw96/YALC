class LedStatus:
    Inactive = int(0)
    Alive = int(1)
    Dead = int(2)

class Color:
    Red = int(0)
    Green = int(0)
    Blue = int(0)

    def __init__(self, red, green, blue):
        self.Red = red
        self.Green = green
        self.Blue = blue

class LedVisitor:
    counter = int(0)
    ledId = int(0)
    status = LedStatus.Inactive
    fadeProps = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1, 1, 0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1]
    color = Color(0,0,0)

    def __init__(self, ledId, color):
        self.counter = 0
        self.ledId = ledId
        self.status = LedStatus.Inactive
        self.color = color

    def fade(self, fade):
        return Color(int(self.color.Red * fade), int(self.color.Green * fade), int(self.color.Blue * fade))

    def nextStep(self, yalc):
        if self.status == LedStatus.Inactive:
            return 0
        elif self.status == LedStatus.Dead:
            return -1
        elif self.status == LedStatus.Alive:
                color = self.fade(self.fadeProps[self.counter])
                yalc.setPixelColor(self.ledId, color.Red, color.Green, color.Blue)
                self.counter += 1
                if self.counter >= len(self.fadeProps):
                    self.status = LedStatus.Dead
                return 1

class LedQueue:
    ledList = []
    queueSize = 7

    def add(self, ledVisitor):
        for led in self.ledList:
            if ledVisitor.ledId == led.ledId:
                return
        self.ledList.append(ledVisitor)

    def remove(self, ledVisitor):
        self.ledList.remove(ledVisitor)

    def isFull(self):
        return len(self.ledList) >= self.queueSize


class Sparks2(YALCAnimation):
    timeElapsed = int(0)
    nextLedTimer = int(0)
    counter = int(0)
    color = [255,255,255]
    timers = [1000000, 1500000, 700000, 1000000, 1100000, 600000]
    queue = LedQueue()


    def __init__(self):
        self.initialise()

    def rand(self, seed):
        return int((seed * 23) + 12345) % 100

    def fade(self):
        for id in range(self.getNumberOfPixels()):
            red = self.color[0] * 0.1
            green = self.color[1] * 0.1
            blue = self.color[2] * 0.1
            self.setPixelColor(id, int(red), int(green), int(blue))

    def initialise(self):
        self.fade()

    def name(self):
        return "Sparks2"

    def periodic(self, timeElapsedInUs):
        randomNum = self.rand(timeElapsedInUs % 17)

        # refill queue
        while not self.queue.isFull():
            self.queue.add(LedVisitor(randomNum, Color(self.color[0], self.color[1], self.color[2])))
            randomNum = self.rand(randomNum)

        # change color and remove dead leds
        if self.timeElapsed > 100000:
            self.timeElapsed = 0
            ledsToRemove = []
            for led in self.queue.ledList:
                if led.nextStep(self) == -1:
                    ledsToRemove.append(led)

            for led in ledsToRemove:
                self.queue.remove(led)


        # take next inactive led
        if self.nextLedTimer > self.timers[self.counter]:
            self.nextLedTimer = 0
            self.counter = (self.counter + 1) % len(self.timers)

            for led in self.queue.ledList:
                if led.status == LedStatus.Inactive:
                    led.status = LedStatus.Alive
                    break

        self.timeElapsed = self.timeElapsed + int(timeElapsedInUs)
        self.nextLedTimer = self.nextLedTimer + int(timeElapsedInUs)

    def getParameters(self):
        return [ColorParameter("color", self.color)]

    def setParameter(self, name, value):
        if name == "color":
            self.color = value
        self.initialise()

def create():
    return Sparks2()