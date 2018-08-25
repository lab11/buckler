I2C IMU App
===========

Reads data from the
[MPU-9250](https://www.invensense.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf)
Inertial Measurement Unit. The chip provides 3-axis accelerometer, gyro, and
magnetometer accessible over I2C. This app also demonstrates an example of how
to utilize local library code. To see output run `make rtt`

Example output:
```
                      X-Axis	    Y-Axis	    Z-Axis
                  ----------	----------	----------
Acceleration (g):     -0.057	     0.001	     1.048
Angle  (degrees):     -3.116	   -27.238	     0.000
Magnetism   (uT):      0.000	    33.000	   -13.800
```

nRF TWIM (Two Wire Interface Master) [API documentation](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.1.0%2Fgroup__nrfx__twim.html)

