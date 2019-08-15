#!/usr/bin/env python3
import struct
import time
import keyboard
from getpass import getpass
from bluepy.btle import Peripheral, DefaultDelegate

YOUR_ADDRESS = "c0:98:e5:49:00:00" # Replace address with your device address
SERVICE_UUID = # TODO: Choose a UUID for your service
CHAR_UUIDS = None # TODO: add your characteristics

class RobotController():

    def __init__(self, address):

        self.robot = Peripheral(YOUR_ADDRESS)
        print("connected")

        # keep state for keypresses
        self.pressed = {"up": False, "down": False, "right": False, "left": False}
        # TODO get service from robot
        # TODO get characteristic handles from service/robot
        # TODO enable notifications if using notifications

        keyboard.hook(self.on_key_event)

    def on_key_event(self, event):
        # print key name
        print(event.name)
        # if a key unrelated to direction keys is pressed, ignore
        if event.name not in self.pressed: return
        # if a key is pressed down
        if event.event_type == keyboard.KEY_DOWN:
            # if that key is already pressed down, ignore
            if self.pressed[event.name]: return
            # set state of key to pressed
            self.pressed[event.name] = True
            # TODO write to characteristic to change direction
        else:
            # set state of key to released
            self.pressed[event.name] = False
            # TODO write to characteristic to stop moving in this direction

    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        self.robot.disconnect()

with RobotController(YOUR_ADDRESS) as robot:
    getpass('Use arrow keys to control robot')
