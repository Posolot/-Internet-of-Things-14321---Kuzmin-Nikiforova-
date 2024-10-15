# publisher
import datetime
import time
import uuid
import serial
import paho.mqtt.client as mqtt_client
import random
from uuid import getnode as get_mac
import hashlib


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
        if cmd == 'p':
            str_resp = str(int.from_bytes(resp, byteorder='little') * 4)
        else:
            str_resp = resp.decode()
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

print("Connecting to broker", broker)
print(client.connect(broker))
client.loop_start()
print("Publishing")

while True:
    photo_val_resp = send_command('p', responses['p'], connection_photo)
    while photo_val_resp == '':
        photo_val_resp = send_command('p', responses['p'], connection_photo)
    photo_val = int(photo_val_resp)
    print(f"Publishing is {photo_val}")
    client.publish(f"lab/{pub_id}/leds/state", photo_val)

    time.sleep(0.3)

client.disconnect()
client.loop_stop()
