import serial
import uuid
import hashlib
import time
import paho.mqtt.client as mqtt_client


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


def on_message(client, userdata, message):
    data = str(message.payload.decode("utf-8"))
    if int(data) >= 750:
        resp = send_command('u', responses['u'], connection_led)
    else:
        resp = send_command('d', responses['d'], connection_led)
    print("received message =", data)


photo_val_resp = ''
responses = {'d': 0,  # led_off
             'u': 0,  # led_on
             }

port_led = "COM3"
connection_led = serial.Serial(port_led, timeout=1)

broker = "broker.emqx.io"

mac = uuid.getnode()
h = hashlib.new('sha256')
h.update(str(mac).encode())
sub_id = h.hexdigest()[:10]
pub_id = 'a5eb8a33d1'


client = mqtt_client.Client(
   mqtt_client.CallbackAPIVersion.VERSION2,
   sub_id
)

client.on_message = on_message

print("Connecting to broker", broker)
client.connect(broker)
client.loop_start()
print("Subcribing")
print(f"lab/{pub_id}/leds/state")
client.subscribe(f"lab/{pub_id}/leds/state")
time.sleep(18100)



