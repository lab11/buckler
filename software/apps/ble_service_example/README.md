BLE Service Example
===================

This example code creates a custom service with readable and writable
characteristics which can be interacted with by a BLE master (such as a
smartphone).

The service and characteristic control LED0. Writing a 0x01 to the
characteristic turns the LED on, writing a 0x00 turns it off.

Using this application is essentially the same as any other Buckler app, with
`make flash` loading the application code and `make rtt` opening the RTT output
viewer.

