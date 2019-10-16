#!/usr/bin/env python3

import struct
from bluepy.btle import Peripheral, DefaultDelegate
import argparse

parser = argparse.ArgumentParser(description='Print advertisement data from a BLE device')
parser.add_argument('addr', metavar='A', type=str, help='Address of the form XX:XX:XX:XX:XX:XX')
args = parser.parse_args()
addr = args.addr.lower()
if len(addr) != 17:
    raise ValueError("Invalid address supplied")

LED_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
LED_CHAR_UUID    = "32e6108a-2b22-4db5-a914-43ce41986c70"


try:
    print("connecting")
    buckler = Peripheral(addr)

    print("connected")

    # Get service
    sv = buckler.getServiceByUUID(LED_SERVICE_UUID)
    # Get characteristic
    ch = sv.getCharacteristics(LED_CHAR_UUID)[0]

    while True:
        input("Press any key to toggle the LED")
        led_state = bool(int(ch.read().hex()))
        ch.write(bytes([not led_state]))
finally:
    buckler.disconnect()
