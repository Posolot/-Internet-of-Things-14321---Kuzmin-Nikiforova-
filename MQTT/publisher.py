# publisher
import datetime
import time
import uuid
import serial
import paho.mqtt.client as mqtt_client
import random
from uuid import getnode as get_mac
import hashlib
from collections import deque
from threading import Thread


photo_val_resp = ''
responses = {'p': 1}

port_photo = "COM8"
connection_photo = serial.Serial(port_photo, timeout=1)

def send_command(
        cmd: str,
        response_len: int,
        connection: serial.Serial
) -> str:
    str_resp: str = ""
    connection.write(cmd.encode())
    if response_len != 0:
        resp: bytes = connection.read(response_len)
        str_resp = str(int.from_bytes(resp, byteorder='little'))
    return str_resp


broker = "broker.emqx.io"

mac = uuid.getnode()
h = hashlib.new('sha256')
h.update(str(mac).encode())
pub_id = h.hexdigest()[:10]
print(pub_id)

client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   pub_id
)

averaging_queue = deque(maxlen=100)
averaging_interval = 2

streaming_on = True
last_stream_on = False
streaming_interval = 0.5

min_value = float('inf')
max_value = float('-inf')
k = 0

print("Connecting to broker", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")


def stream_on():
    while True:
        photo_val_resp = send_command('p', responses['p'], connection_photo)
        while photo_val_resp == '':
            photo_val_resp = send_command('p', responses['p'], connection_photo)
        photo_val = int(photo_val_resp)
        print(f"Publishing stream: {photo_val}")
        client.publish(f"lab/{pub_id}/photo/stream", photo_val)
        time.sleep(streaming_interval)


while True:

    photo_val_resp = send_command('p', responses['p'], connection_photo)
    while photo_val_resp == '':
        photo_val_resp = send_command('p', responses['p'], connection_photo)
    photo_val = int(photo_val_resp)
    print(f"Publishing is {photo_val}")
    client.publish(f"lab/{pub_id}/photo/instant", photo_val)

    averaging_queue.append(photo_val)
    if len(averaging_queue) == 100:
        average = sum(averaging_queue) / len(averaging_queue)
        print(f"Publishing average: {average}")
        client.publish(f"lab/{pub_id}/photo/average", average)

    if streaming_on and last_stream_on != streaming_on:
        stream_thread = Thread(target=stream_on)
        stream_thread.start()
        last_stream_on = streaming_on
    else:
        if 'instant_thread' in locals() and stream_thread.is_alive():
            stream_thread.join()
            last_stream_on = streaming_on


  

    if photo_val < min_value and photo_val > 0:
        min_value = photo_val
    if photo_val > max_value:
        max_value = photo_val
    if k == 5:
        print(f"Publishing max and min: {max_value}, {min_value}")
        client.publish(f"lab/{pub_id}/photo/min", min_value)
        client.publish(f"lab/{pub_id}/photo/max", max_value)
        k = 0

    k += 1
    time.sleep(1)



client.disconnect()
client.loop_stop()

