import socket
import sys
from datetime import datetime, timedelta
import copy
from YALCDaemon import YALCDaemon, YALCRGB

if __name__ == "__main__":
    if len(sys.argv) == 3:
        # Get "IP address of Server" and also the "port number" from
        # argument 1 and argument 2
        ip = sys.argv[1]
        port = int(sys.argv[2])
    else:
        print("Run like : python3 server.py <arg1:server ip:this system IP 192.168.1.6> <arg2:server port:4444 >")
        exit(1)

    print(f"IP: {ip}, port: {port}")
    daemon = YALCDaemon(ip, port)

    colors = [(255,0,0), (0,0,255), (0,255,0)]
    steps = 25
    step = 0
    timeout = timedelta(milliseconds=250)

    color_id = 1
    delta = (int((colors[1][0] - colors[0][0]) / steps), int((colors[1][1] - colors[0][1]) / steps), int((colors[1][2] - colors[0][2]) / steps))
    print("delta", delta)
    change_dt = datetime.now()
    color = colors[0]

    while True:
        daemon.periodic()

        dt_now = datetime.now()
        if dt_now - timeout > change_dt:
            print(color)
            yalc_color = YALCRGB(color[0], color[1], color[2])

            for node_id in daemon.get_nodes_list():
                for led_id in range(daemon.get_number_of_leds(node_id)):
                    daemon.set_node_color(node_id, led_id, yalc_color)

            color = (color[0] + delta[0], color[1] + delta[1], color[2] + delta[2])
            step += 1
            if step == steps:
                next_color = color_id+1
                if color_id == len(colors)-1:
                    next_color = 0

                print(next_color, color_id)
                delta = (int((colors[next_color][0] - colors[color_id][0]) / steps), int((colors[next_color][1] - colors[color_id][1]) / steps), int((colors[next_color][2] - colors[color_id][2]) / steps))
                color = colors[color_id]
                step = 0
                print("delta", delta)
                print("colors:", colors[next_color], colors[color_id])
                color_id = next_color


            change_dt = datetime.now()



