/******************************************************************************
Neal Jackson
Datasheet: https://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf
Adapted from Sparkfun Driver:
https://raw.githubusercontent.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library/master/src/LSM9DS1_Types.h
******************************************************************************/

#pragma once

#include "nrf_twi_mngr.h"

#include "lsm9ds1_registers.h"

// Sensor Sensitivity Constants
// Values set according to the typical specifications provided in
// table 3 of the LSM9DS1 datasheet. (pg 12)
#define SENSITIVITY_ACCELEROMETER_2  0.000061
#define SENSITIVITY_ACCELEROMETER_4  0.000122
#define SENSITIVITY_ACCELEROMETER_8  0.000244
#define SENSITIVITY_ACCELEROMETER_16 0.000732
#define SENSITIVITY_GYROSCOPE_245    0.00875
#define SENSITIVITY_GYROSCOPE_500    0.0175
#define SENSITIVITY_GYROSCOPE_2000   0.07
#define SENSITIVITY_MAGNETOMETER_4   0.00014
#define SENSITIVITY_MAGNETOMETER_8   0.00029
#define SENSITIVITY_MAGNETOMETER_12  0.00043
#define SENSITIVITY_MAGNETOMETER_16  0.00058

typedef struct {
  float x_axis;
  float y_axis;
  float z_axis;
} lsm9ds1_measurement_t;

typedef enum {
  X_AXIS,
  Y_AXIS,
  Z_AXIS,
  ALL_AXIS
} lsm9ds1_axis;

// The LSM9DS1 functions over both I2C or SPI. This library supports both.
// But the interface mode used must be sent to the LSM9DS1 constructor. Use
// one of these two as the first parameter of the constructor.

typedef enum
{
    IMU_MODE_SPI,
    IMU_MODE_I2C,
} interface_mode;

// accel_scale defines all possible FSR's of the accelerometer:
typedef enum
{
    A_SCALE_2G,  // 00:  2g
    A_SCALE_16G, // 01:  16g
    A_SCALE_4G,  // 10:  4g
    A_SCALE_8G   // 11:  8g
} accel_scale;

// gyro_scale defines the possible full-scale ranges of the gyroscope:
typedef enum
{
    G_SCALE_245DPS,  // 00:  245 degrees per second
    G_SCALE_500DPS,  // 01:  500 dps
    G_SCALE_2000DPS, // 11:  2000 dps
} gyro_scale;

// mag_scale defines all possible FSR's of the magnetometer:
typedef enum
{
    M_SCALE_4GS,  // 00:  4Gs
    M_SCALE_8GS,  // 01:  8Gs
    M_SCALE_12GS, // 10:  12Gs
    M_SCALE_16GS, // 11:  16Gs
} mag_scale;

// gyro_odr defines all possible data rate/bandwidth combos of the gyro:
typedef enum
{
    G_ODR_PD,  // Power down (0)
    G_ODR_149, // 14.9 Hz (1)
    G_ODR_595, // 59.5 Hz (2)
    G_ODR_119, // 119 Hz (3)
    G_ODR_238, // 238 Hz (4)
    G_ODR_476, // 476 Hz (5)
    G_ODR_952  // 952 Hz (6)
} gyro_odr;

// accel_oder defines all possible output data rates of the accelerometer:
typedef enum
{
    XL_POWER_DOWN, // Power-down mode (0x0)
    XL_ODR_10,     // 10 Hz (0x1)
    XL_ODR_50,     // 50 Hz (0x02)
    XL_ODR_119,    // 119 Hz (0x3)
    XL_ODR_238,    // 238 Hz (0x4)
    XL_ODR_476,    // 476 Hz (0x5)
    XL_ODR_952     // 952 Hz (0x6)
} accel_odr;

// accel_abw defines all possible anti-aliasing filter rates of the accelerometer:
typedef enum
{
    A_ABW_408, // 408 Hz (0x0)
    A_ABW_211, // 211 Hz (0x1)
    A_ABW_105, // 105 Hz (0x2)
    A_ABW_50,  //  50 Hz (0x3)
} accel_abw;

// mag_odr defines all possible output data rates of the magnetometer:
typedef enum
{
    M_ODR_0625, // 0.625 Hz (0)
    M_ODR_125,  // 1.25 Hz (1)
    M_ODR_250,  // 2.5 Hz (2)
    M_ODR_5,    // 5 Hz (3)
    M_ODR_10,   // 10 Hz (4)
    M_ODR_20,   // 20 Hz (5)
    M_ODR_40,   // 40 Hz (6)
    M_ODR_80    // 80 Hz (7)
} mag_odr;

typedef enum
{
    XG_INT1 = INT1_CTRL,
    XG_INT2 = INT2_CTRL
} interrupt_select;

typedef enum
{
    INT_DRDY_XL = (1 << 0),    // Accelerometer data ready (INT1 & INT2)
    INT_DRDY_G = (1 << 1),     // Gyroscope data ready (INT1 & INT2)
    INT1_BOOT = (1 << 2),      // Boot status (INT1)
    INT2_DRDY_TEMP = (1 << 2), // Temp data ready (INT2)
    INT_FTH = (1 << 3),           // FIFO threshold interrupt (INT1 & INT2)
    INT_OVR = (1 << 4),           // Overrun interrupt (INT1 & INT2)
    INT_FSS5 = (1 << 5),       // FSS5 interrupt (INT1 & INT2)
    INT_IG_XL = (1 << 6),      // Accel interrupt generator (INT1)
    INT1_IG_G = (1 << 7),      // Gyro interrupt enable (INT1)
    INT2_INACT = (1 << 7),     // Inactivity interrupt output (INT2)
} interrupt_generators;

typedef enum
{
    XLIE_XL = (1 << 0),
    XHIE_XL = (1 << 1),
    YLIE_XL = (1 << 2),
    YHIE_XL = (1 << 3),
    ZLIE_XL = (1 << 4),
    ZHIE_XL = (1 << 5),
    GEN_6D = (1 << 6)
} accel_interrupt_generator;

typedef enum
{
    XLIE_G = (1 << 0),
    XHIE_G = (1 << 1),
    YLIE_G = (1 << 2),
    YHIE_G = (1 << 3),
    ZLIE_G = (1 << 4),
    ZHIE_G = (1 << 5)
} gyro_interrupt_generator;

typedef enum
{
    ZIEN = (1 << 5),
    YIEN = (1 << 6),
    XIEN = (1 << 7)
} mag_interrupt_generator;

typedef enum
{
    INT_ACTIVE_HIGH,
    INT_ACTIVE_LOW
} h_lactive;

typedef enum
{
    INT_PUSH_PULL,
    INT_OPEN_DRAIN
} pp_od;

typedef enum
{
    FIFO_OFF = 0,
    FIFO_THS = 1,
    FIFO_CONT_TRIGGER = 3,
    FIFO_OFF_TRIGGER = 4,
    FIFO_CONT = 6
} fifoMode_type;

typedef struct
{
    // Gyroscope settings:
    uint8_t enabled;
    gyro_scale scale;
    uint8_t sampleRate;
    // New gyro stuff:
    uint8_t bandwidth;
    uint8_t lowPowerEnable;
    uint8_t HPFEnable;
    uint8_t HPFCutoff;
    uint8_t flipX;
    uint8_t flipY;
    uint8_t flipZ;
    uint8_t orientation;
    uint8_t enableX;
    uint8_t enableY;
    uint8_t enableZ;
    uint8_t latchInterrupt;
} gyroSettings;

typedef struct
{
    uint8_t commInterface; // Can be I2C, SPI 4-wire or SPI 3-wire
    uint8_t agAddress;     // I2C address or SPI CS pin
    uint8_t mAddress;      // I2C address or SPI CS pin
    const nrf_twi_mngr_t * i2c;  // pointer to an instance of I2C interface
} deviceSettings;

typedef struct
{
    // Accelerometer settings:
    uint8_t enabled;
    accel_scale scale;
    uint8_t sampleRate;
    // New accel stuff:
    uint8_t enableX;
    uint8_t enableY;
    uint8_t enableZ;
    int8_t bandwidth;
    uint8_t highResEnable;
    uint8_t highResBandwidth;
} accelSettings;

typedef struct
{
    // Magnetometer settings:
    uint8_t enabled;
    mag_scale scale;
    uint8_t sampleRate;
    // New mag stuff:
    uint8_t tempCompensationEnable;
    uint8_t XYPerformance;
    uint8_t ZPerformance;
    uint8_t lowPowerEnable;
    uint8_t operatingMode;
} magSettings;

typedef struct
{
    // Temperature settings
    uint8_t enabled;
} temperatureSettings;

typedef struct
{
    deviceSettings device;

    gyroSettings gyro;
    accelSettings accel;
    magSettings mag;

    temperatureSettings temp;
} IMUSettings;
