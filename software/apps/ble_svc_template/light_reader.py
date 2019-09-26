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

LIGHT_SERVICE_UUID = "de97aeee-0e7e-4720-8038-4dc47aa9562f"
LIGHT_CHAR_UUID = "de97aeef-0e7e-4720-8038-4dc47aa9562f"


class CharDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        [lux] = struct.unpack('<f', data)
        print("Value: " + str(lux) + " lux")

try:
    print("connecting")
    buckler = Peripheral(addr)
    buckler.setDelegate(CharDelegate())
    print("connected")

    sv = buckler.getServiceByUUID(LIGHT_SERVICE_UUID)

    # Get characteristic
    ch = sv.getCharacteristics(LIGHT_CHAR_UUID)[0]
    # Enable notifications
    buckler.writeCharacteristic(ch.valHandle+1, b"\x01\x00")

    while True:
        if buckler.waitForNotifications(2.0):
            print("Got notification")
finally:
    buckler.disconnect()
