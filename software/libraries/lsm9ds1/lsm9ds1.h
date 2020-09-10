/******************************************************************************
Neal Jackson
Datasheet: https://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf
Adapted from Sparkfun Driver:
https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library/blob/master/src/SparkFunLSM9DS1.h
******************************************************************************/

#pragma once

#include "app_error.h"
#include "nrf_twi_mngr.h"

#include "buckler.h"
#include "lsm9ds1_registers.h"
#include "lsm9ds1_types.h"

// Initialize and configure the lsm9ds1
//
// i2c - pointer to already initialized and enabled twim instance
ret_code_t lsm9ds1_init(const nrf_twi_mngr_t* i2c);

// Read all three axes on the accelerometer
//
// Return measurements as floating point values in g's
lsm9ds1_measurement_t lsm9ds1_read_accelerometer();

// Read all three axes on the gyro
//
// Return measurements as floating point values in degrees/second
lsm9ds1_measurement_t lsm9ds1_read_gyro();

// Read all three axes on the magnetometer
//
// Return measurements as floating point values in uT
lsm9ds1_measurement_t lsm9ds1_read_magnetometer();

// Start integration on the gyro
//
// Return an NRF error code
//  - must be stopped before starting
ret_code_t lsm9ds1_start_gyro_integration();

// Stop integration on the gyro
void lsm9ds1_stop_gyro_integration();

// Read the value of the integrated gyro
//
// Note: this function also performs the integration and needs to be called
// periodically
//
// Return the integrated value as floating point in degrees
lsm9ds1_measurement_t lsm9ds1_read_gyro_integration();

