BLE Service Example
===================

This example code creates a custom service with readable and writable
characteristics which can be interacted with by a BLE master (such as a
smartphone). The majority of this code was copied from
[Custom BLE Service Example](https://github.com/bjornspockeli/custom_ble_service_example),
check out that repository for explanations of each part of the code.


Note that the current contents of this app are a hack. _Something_ is wrong
with the way the simple BLE library and the nrf5x-base build system sets up
BLE, resulting in connections that disconnect immediately and do not function
correctly. Instead, this app uses the SDK15 example build system to create an
application, with the Makefile, SDK config, and LD files copied directly into
this folder.

This application may or may not play well with various libraries used on
Buckler. I expect that the sensor libraries and Kobuki driver will likely work.
If you run into issues, let us know.

Using this application is essentially the same as any other Buckler app, with
`make flash` loading the application code and `make rtt` opening the RTT output
viewer.

