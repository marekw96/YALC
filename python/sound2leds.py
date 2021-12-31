import pyaudio
import struct
import time
import math
import socket
import colorsys

def rms( data ):
    count = len(data)/2
    format = "%dh"%(count)
    shorts = struct.unpack( format, data )
    sum_squares = 0.0
    for sample in shorts:
        n = sample * (1.0/32768)
        sum_squares += n*n
    return math.sqrt( sum_squares / count )

audio = pyaudio.PyAudio()

print("----------------------record device list---------------------")
info = audio.get_host_api_info_by_index(0)
numdevices = info.get('deviceCount')
for i in range(0, numdevices):
        if (audio.get_device_info_by_host_api_device_index(0, i).get('maxInputChannels')) > 0:
            print("Input Device id ", i, " - ", audio.get_device_info_by_host_api_device_index(0, i).get('name'))

print("-------------------------------------------------------------")


NUM_LEDS = 25
color = 0
color_step = 0.002
reset_threshold = 5
reset_counter = 0
leds = bytearray(NUM_LEDS*3)

udp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp.bind(("192.168.0.24", 7777))

print("Waiting for client to connect:")
data, client_info = udp.recvfrom(1024)
print(data, client_info)

RATE = 48000
CHUNK = 512 * 2

p = pyaudio.PyAudio()
stream = p.open(
            format=pyaudio.paInt16,
            channels=1,
            rate=RATE,
            input=True,
            output=False,
            frames_per_buffer=CHUNK,
        )


start_time = time.time()
frames = 0

min_val = 100
max_val = 1

avg = 0

while True:
    frames = frames+1

    data = stream.read(CHUNK)
    data_int = struct.unpack(str(2 * CHUNK) + 'B', data)

    rms_value = rms(data)
    decibel = 20 * math.log10(rms_value)
    rev_decibel = 100 + decibel
    min_val = min(min_val, rev_decibel)
    max_val = max(max_val, rev_decibel)
    val_diff = max_val - min_val
    fill = 0
    if val_diff != 0:
        fill = (rev_decibel - min_val) / val_diff
        percent = int(fill * NUM_LEDS)
        percent = max(1, percent)
        avg = avg + percent
        leds = bytearray(NUM_LEDS*3)

        (r,g,b) = colorsys.hsv_to_rgb(color, 1.0, 1.0)
        color = color + color_step
        if color > 1.0:
            color = 0

        for i in range(percent):
            leds[i * 3] = int(255 * r)
            leds[i * 3 + 1] = int(255 * g)
            leds[i * 3 + 2] = int(255 * b)
        udp.sendto(leds, client_info)
        #fill = fill * 100


    # print (rms_value, decibel, 100 + decibel, min_val, max_val, fill)
    if time.time() - start_time >= 1 :
        avg = avg / frames
        print(f"Frames: {frames}, min_val: {min_val}, max_val: {max_val}, avg: {avg}")
        start_time = time.time()
        frames = 0

        avg_fill = avg / NUM_LEDS * 100
        if avg < 8:
            reset_counter = reset_counter +1
            if reset_counter >= reset_threshold:
                min_val = 100
                max_val = 1
                print("Min max reset")

        reset_counter = 0

