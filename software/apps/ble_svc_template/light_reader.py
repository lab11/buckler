import struct
from bluepy.btle import Peripheral, DefaultDelegate

class CharDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        [lux] = struct.unpack('<f', data)
        print("Value: " + str(lux) + " lux")

YOUR_ADDRESS = "c0:98:e5:49:00:00" # Replace address with your device address
YOUR_SERVICE_UUID = "de97aeee-0e7e-4720-8038-4dc47aa9562f"
YOUR_CHAR_UUID = "de97aeef-0e7e-4720-8038-4dc47aa9562f"

try:
    buckler = Peripheral(YOUR_ADDRESS)
    buckler.setDelegate(CharDelegate())

    print("connected")

    sv = buckler.getServiceByUUID(YOUR_SERVICE_UUID)

    # Enable notifications:
    ch = sv.getCharacteristics(YOUR_CHAR_UUID)[0]
    buckler.writeCharacteristic(ch.valHandle+1, b"\x01\x00")

    while True:
        if buckler.waitForNotifications(2.0):
            print("Got notification")
finally:
    buckler.disconnect()
