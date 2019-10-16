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

DISPLAY_SERVICE_UUID = "32e61089-2b22-4db5-a914-43ce41986c70"
DISPLAY_CHAR_UUID    = "32e6108a-2b22-4db5-a914-43ce41986c70"


try:
    print("connecting")
    buckler = Peripheral(addr)

    print("connected")

    # Get service
    sv = buckler.getServiceByUUID(DISPLAY_SERVICE_UUID)
    # Get characteristic
    ch = sv.getCharacteristics(DISPLAY_CHAR_UUID)[0]

    while True:
        display = input("Enter a message to write to the display:\n")
        ch.write(bytes(display, 'utf-8'))
finally:
    buckler.disconnect()
