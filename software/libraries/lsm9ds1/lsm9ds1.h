/******************************************************************************
Neal Jackson
Datasheet: https://www.st.com/content/ccc/resource/technical/document/datasheet/1e/3f/2a/d6/25/eb/48/46/DM00103319.pdf/files/DM00103319.pdf/jcr:content/translations/en.DM00103319.pdf
Adapted from Sparkfun Driver:
https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library/blob/master/src/SparkFunLSM9DS1.h
******************************************************************************/

#pragma once

#include "app_error.h"
#include "nrf_twi_mngr.h"

#include "lsm9ds1_registers.h"
#include "lsm9ds1_types.h"

// Initialize and configure the lsm9ds1
//
// i2c - pointer to already initialized and enabled twim instance
ret_code_t lsm9ds1_init(uint8_t agAddress, uint8_t mAddress, const nrf_twi_mngr_t* i2c);

lsm9ds1_measurement lsm9ds1_read_accelerometer();
lsm9ds1_measurement lsm9ds1_read_gyro();
lsm9ds1_measurement lsm9ds1_read_magnetometer();
ret_code_t lsm9ds1_start_gyro_integration();
void lsm9ds1_stop_gyro_integration();
lsm9ds1_measurement lsm9ds1_read_gyro_integration();

// Read whoami from accelerometer
uint8_t lsm9ds1_whoami_ag();
// Read whoami from magnetometer
uint8_t lsm9ds1_whoami_m();
