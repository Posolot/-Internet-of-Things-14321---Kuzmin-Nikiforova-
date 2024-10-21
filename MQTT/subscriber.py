import serial
import uuid
import hashlib
import time
import paho.mqtt.client as mqtt_client

min_avg = 0
max_avg = 255

window_size = 5
measurements = []

data_average = {}
data_instant = {}

streaming_on = False


def avg(data_list):
    if not data_list:
        return 0
    return sum(data_list) / len(data_list)


def send_command(cmd: str, response_len: int, connection: serial.Serial) -> str:
    str_resp: str = ""
    connection.write(cmd.encode())
    if response_len != 0:
        resp: bytes = connection.read(response_len)
        if cmd == 'p':
            str_resp = str(int.from_bytes(resp, byteorder='little') * 4)
        else:
            str_resp = resp.decode()
    return str_resp


def detect_trend(data_list):
    if len(data_list) < window_size:
        return None

    is_decreasing = all(x > y for x, y in zip(data_list, data_list[1:]))
    is_increasing = all(x < y for x, y in zip(data_list, data_list[1:]))

    if is_decreasing:
        return 'decreasing'
    elif is_increasing:
        return 'increasing'
    return None


def on_message(client, userdata, message):
    global min_avg, max_avg, streaming_on
    data = str(message.payload.decode("utf-8"))

    if message.topic == f"lab/{pub_id}/photo/instant":
        data_instant[time.time()] = int(data)
        print("Instant data received:", data)

    elif message.topic == f"lab/{pub_id}/photo/average":
        data_average[time.time()] = float(data)
        print("Average data received:", data)

    elif message.topic == f"lab/{pub_id}/photo/stream":
        print(f"Stream data: {data}")
        measurements.append(int(data))
        if len(measurements) > window_size:
            measurements.pop(0)
        if len(measurements) >= 1:
            increasing = all(measurements[i] < measurements[i + 1] for i in range(len(measurements) - 1))
            decreasing = all(measurements[i] > measurements[i + 1] for i in range(len(measurements) - 1))

            if int(data)-2 <= min_avg:
                resp = send_command('d', responses['d'], connection_led)
            elif int(data)+2 >= max_avg:
                resp = send_command('u', responses['u'], connection_led)
            elif increasing:
                print("Значения растут, включаем светодиод.")
                resp = send_command('u', responses['u'], connection_led)
            elif decreasing:
                print("Значения падают, выключаем светодиод.")
                resp = send_command('d', responses['d'], connection_led)

    elif message.topic == f"lab/{pub_id}/photo/min":
        min_avg = min_avg if min_avg < int(data) else int(data)
        print(f"Updated min_avg: {min_avg}")

    elif message.topic == f"lab/{pub_id}/photo/max":
        max_avg = max_avg if max_avg > int(data) else int(data)
        print(f"Updated max_avg: {max_avg}")


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
pub_id = 'defbc95e57'

client = mqtt_client.Client(
    mqtt_client.CallbackAPIVersion.VERSION2,
    sub_id
)

client.on_message = on_message

print("Connecting to broker", broker)
client.connect(broker)
print("Subcribing")
client.loop_start()
client.subscribe(f"lab/{pub_id}/photo/instant")
print(f"Subcribing lab/{pub_id}/photo/instant")
client.subscribe(f"lab/{pub_id}/photo/average")
print(f"Subcribing lab/{pub_id}/photo/average")
client.subscribe(f"lab/{pub_id}/photo/stream")
print(f"Subcribing lab/{pub_id}/photo/stream")
client.subscribe(f"lab/{pub_id}/photo/min")
print(f"Subcribing lab/{pub_id}/photo/min")
client.subscribe(f"lab/{pub_id}/photo/max")
print(f"Subcribing lab/{pub_id}/photo/max")
client.subscribe(f"lab/{pub_id}/photo/activate_stream")
print(f"Subcribing lab/{pub_id}/photo/activate_stream")

time.sleep(18100)
client.disconnect()
client.loop_stop()
