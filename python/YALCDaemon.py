import socket
import sys
from datetime import datetime, timedelta
import copy

def str_to_sized_bytearray(txt, size):
    padding = 0
    if len(txt) > size:
        txt = txt[0:size]
    else:
        padding = size - len(txt)

    data = bytearray(map(ord, txt))
    data.extend([0 for i in range(padding)])

    return data

class YALCMessages:
    HANDSHAKE = 1
    HANDSHAKE_REPLY = 2
    I_AM_ALIVE = 3
    COLOR_LEDS_8_BIT = 0x11

class YALCRGB:
    R = 0
    G = 1
    B = 2

    def __init__(self, r=0, g=0, b=0):
        self.components = [r,g,b]

    def get(self, pos):
        return self.components[pos]

    def update(self, new_color):
        self.components = copy.copy(new_color.components)

class YALCNode:
    PERIODIC_SUCCESS = 1
    PERIODIC_DISCONNECT = 2

    def __init__(self, node_id, address, s):
        self.node_id = node_id
        self.address = address
        self.s = s
        self.parameters = {}
        dt_now = datetime.now()

        self.timestamp = {"alive_indicator": dt_now, "send_update": dt_now}
        self.sequence = {"COLOR_LEDS_8_BIT": 1}

    def get_device_id(self):
        return self.parameters['device_id']

    def handle_handshake(self, packet):
        data = packet[4:]
        device_id = int.from_bytes(data[0:4], "big")
        sw_major, sw_minor, sw_path = data[4], data[5], data[6]
        api_major, api_minor, api_path = data[7], data[8], data[9]
        number_of_leds = int.from_bytes(data[10:12], "big")
        led_config = data[12]
        mtu = int.from_bytes(data[13:15], "big")
        leds_per_meter = data[15]
        name = data[16:56].decode("utf-8")

        self.parameters['device_id'] = device_id
        self.parameters['number_of_leds'] = number_of_leds
        self.leds = [YALCRGB() for i in range(number_of_leds)]
        self.parameters['name'] = name
        self.parameters['mtu'] = mtu

        print(f"HANDSHAKE: device_id: {device_id}, sw: {sw_major}.{sw_minor}.{sw_path}, api: {api_major}.{api_minor}.{api_path}, leds: {number_of_leds}, config: {led_config}, mtu: {mtu}, ppm: {leds_per_meter}, name: {name}")
        self.send_handshake_reply()

    def send_handshake_reply(self):
        data = self._make_header(YALCMessages.HANDSHAKE_REPLY, 1)
        data += self.get_device_id().to_bytes(4, "big")
        data += (0xffeeddcc).to_bytes(4, "big")
        data += (1).to_bytes(3, "big")
        data += (4).to_bytes(3, "big")
        data += (0).to_bytes(2, "big")
        data += (1500).to_bytes(2, "big")
        data += str_to_sized_bytearray("YALC Daemon", 40)
        data += self.node_id.to_bytes(1, "big")

        print("send_handshake_reply", self.node_id, data, len(data))
        self.s.sendto(data, self.address)
        self.timestamp["alive_indicator"]  = datetime.now() + timedelta(seconds=3)

    def send_leds_8bit(self):
        data = self._make_header(YALCMessages.COLOR_LEDS_8_BIT, self.sequence["COLOR_LEDS_8_BIT"])
        data += (0).to_bytes(2, "big")
        data += self.parameters['number_of_leds'].to_bytes(2, "big")

        for led_color in self.leds:
            for component in range(3):
                data += led_color.get(component).to_bytes(1, "big")

        # print("send_leds_8bit", self.address, self.node_id, self.sequence["COLOR_LEDS_8_BIT"], data, len(data))
        self.s.sendto(data, self.address)
        self.timestamp["send_update"]  = datetime.now() + timedelta(milliseconds=40)
        self.sequence["COLOR_LEDS_8_BIT"] += 1

    def handle_i_am_alive(self, packet):
        self.timestamp["alive_indicator"]  = datetime.now() + timedelta(seconds=5)

    def _make_header(self, message_id, sequence_id):
        data = bytearray()
        data += self.node_id.to_bytes(1, "big")
        data += message_id.to_bytes(1, "big")
        data += (sequence_id%(2**16-1)).to_bytes(2, "big")

        return data

    def periodic(self):
        dt_now = datetime.now()

        if dt_now > self.timestamp['send_update']:
            self.send_leds_8bit()

        if dt_now > self.timestamp['alive_indicator']:
            print("Node should be disconnected")
            return YALCNode.PERIODIC_DISCONNECT

        return YALCNode.PERIODIC_SUCCESS

    def set_color(self, pixel_id, new_color):
        if pixel_id < self.parameters['number_of_leds']:
            self.leds[pixel_id].update(new_color)

    def get_number_of_leds(self):
        return self.parameters['number_of_leds']

class YALCDaemon:
    def __init__(self, ip, port):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.s.bind((ip, port))
        self.s.setblocking(False)

        self.nodes = {}
        self.last_node_id = 0

    def periodic(self):
        nodes_to_remove = []

        for node_id in self.nodes:
            result = self.nodes[node_id].periodic()
            if result == YALCNode.PERIODIC_DISCONNECT:
                nodes_to_remove.append(node_id)

        for node_id in nodes_to_remove:
            self.remove_node(node_id)

        data = []
        address = ""
        try:
            data, address = self.s.recvfrom(4096)
        except:
            return

        # if data:
        d = " ".join(hex(i) for i in data)
        #print(f"Got data: {d}")
        node_id = data[0] #int.from_bytes(data[0], "big")
        message_id = data[1] #int.from_bytes(data[1], "big")
        sequence_id = int.from_bytes(data[2:3], "big")
        print(f"INCOMING MSG: node:{node_id} msg_id:{message_id} seq_id:{sequence_id}")

        if message_id == YALCMessages.HANDSHAKE:
            self.handle_handshake(data, address)
        if message_id == YALCMessages.I_AM_ALIVE:
            node = self.find_node(node_id)
            if node:
                node.handle_i_am_alive(data)

    def remove_node(self, node_id):
        node = self.nodes.pop(node_id, None)
        if node:
            print(f"Removed {node_id} node because perioid return DISCONNECT command")
        else:
            print(f"Node {node_id} should be removed because periodic returned DICONNECT command but node was not found")

    def find_node(self, node_id):
        if node_id in self.nodes:
            return self.nodes[node_id]

        return None

    def handle_handshake(self, data, address):
        self.last_node_id += 1
        node = YALCNode(self.last_node_id, address, self.s)

        node.handle_handshake(data)
        self.nodes[self.last_node_id] = node
        print(f"New node {self.last_node_id} attached")

    def set_node_color(self, node_id, pixel_id, new_color):
        node = self.find_node(node_id)
        if node:
            node.set_color(pixel_id, new_color)

    def get_nodes_list(self):
        return list(self.nodes.keys())

    def get_number_of_leds(self, node_id):
        node = self.find_node(node_id)
        if node:
            return node.get_number_of_leds()

        return -1

