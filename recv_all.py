import time
import datetime

def connect_and_transmit(client):
    header = generate_header(0, 0x2, 1)
    last_client_activity = datetime.datetime.now()
    s.sendto(header, client)
    s.setblocking(0)
    i = 0
    while True:
        pixels = generate_header(0, 0xC8, 1) + generate_setpixels(0, [[i,i,i] for id in range(10)])
        s.sendto(pixels, client)
        time.sleep(1.0/30)
        i = (i + 1) % 255
        try:
            pkt = s.recvfrom(1024)
            print(pkt)
            if pkt[0][1] == 0x3:
                last_client_activity = datetime.datetime.now()
        except:
            pass

        if datetime.datetime.now() - last_client_activity > datetime.timedelta(seconds=3):
            print("No last activity. exit loop")
            break

    s.setblocking(1)

def generate_header(node_id, message_id, sequence_id):
    return node_id.to_bytes(1, 'big') + message_id.to_bytes(1, 'big') + sequence_id.to_bytes(2, 'big')

def generate_setpixels(start_pixel, pixels):
    d = start_pixel.to_bytes(4, 'big') + len(pixels).to_bytes(4, 'big')
    for rgb in pixels:
        d += rgb[0].to_bytes(1, "big")
        d += rgb[1].to_bytes(1, "big")
        d += rgb[2].to_bytes(1, "big")

    return d

from socket import *
s=socket(AF_INET, SOCK_DGRAM)
s.bind(('',5432))
while True:
    m=s.recvfrom(1024)
    print(m)
    connect_and_transmit(m[1])