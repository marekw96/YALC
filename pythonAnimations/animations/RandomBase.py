RND_MODULUS = 524287
RND_A = 283741
RND_C = 0
RND_SEED = 3007

def rand(max_value = 0):
	global RND_SEED
	RND_SEED = (RND_A * RND_SEED + RND_C) % RND_MODULUS
	if max_value != 0:
		return RND_SEED % max_value
	return RND_SEED

def randrange(start, stop):
	return rand(stop - start) + start

def shuffle(data):
	size = len(data)
	free_ids = [id for id in range(size)]
	new_data = []

	for iteration in range(size):
		id = free_ids[rand(size)]
		size -= 1
		new_data.append(data[id])
		free_ids.remove(id)

	return new_data


class RandomBase(YALCAnimation):
	def name(self):
		return "RandomBase"

	def periodic(self, timeElapsedInUs):
		self.fill([randrange(0,255), randrange(0,255), randrange(0,255)])

	def fill(self, color):
		for i in range(self.getNumberOfPixels()):
			self.setPixelColor(i, color[0], color[1], color[2])


def create():
	return RandomBase()