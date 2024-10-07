import serial
# import time


photo_val_resp = ''
responses = {'d': 0,  # led_off
             'u': 0,  # led_on
             'p': 1}  # zero fill to left_side  - only for photo_resistor

port_led = "COM3"
port_photo = "COM8"
connection_led = serial.Serial(port_led, timeout=1)
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


while True:
    photo_val_resp = send_command('p', responses['p'], connection_photo)
    while photo_val_resp == '':
        photo_val_resp = send_command('p', responses['p'], connection_photo)
    photo_val = int(photo_val_resp)
    # print(photo_val)
    if photo_val >= 700:
        # print('ON')
        resp = send_command('u', responses['u'], connection_led)
    else:
        # print('OFF')
        resp = send_command('d', responses['d'], connection_led)
