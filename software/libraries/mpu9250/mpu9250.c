// MPU-9250 driver
//
// Read from MPU-9250 3-axis accelerometer/gyro/magnetometer over I2C
// Register documentation: http://www.invensense.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf
//
// Based on https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library/blob/master/src/util/inv_mpu.c

#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_twim.h"

#include "mpu9250.h"

static uint8_t MPU_ADDRESS = 0x68;
static uint8_t MAG_ADDRESS = 0x0C;

static nrfx_twim_t* i2c_instance = NULL;

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TXRX(i2c_addr,
      &reg_addr, 1, &rx_buf, 1);
  ret_code_t error_code = nrfx_twim_xfer(i2c_instance, &xfer, 0);
  APP_ERROR_CHECK(error_code);
  return rx_buf;
}

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = {reg_addr, data};
  nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TX(i2c_addr, buf, 2);
  ret_code_t error_code = nrfx_twim_xfer(i2c_instance, &xfer, 0);
  APP_ERROR_CHECK(error_code);
}

// initialization and configuration
void mpu9250_init(nrfx_twim_t* i2c) {
  i2c_instance = i2c;

  // reset mpu
  i2c_reg_write(MPU_ADDRESS, MPU9250_PWR_MGMT_1, 0x80);
  nrf_delay_ms(100);

  // disable sleep mode
  i2c_reg_write(MPU_ADDRESS, MPU9250_PWR_MGMT_1, 0x00);

  // enable bypass mode
  i2c_reg_write(MPU_ADDRESS, MPU9250_USER_CTRL, 0x00);
  nrf_delay_ms(3);
  i2c_reg_write(MPU_ADDRESS, MPU9250_INT_PIN_CFG, 0x02);

  // configure gyro range to +/- 2000 degrees per second
  i2c_reg_write(MPU_ADDRESS, MPU9250_GYRO_CONFIG, 0x18);

  // configure accelerometer range to +/- 2 g
  i2c_reg_write(MPU_ADDRESS, MPU9250_ACCEL_CONFIG, 0x00);

  // reset magnetometer
  i2c_reg_write(MAG_ADDRESS, AK8963_CNTL2, 0x01);
  nrf_delay_ms(100);

  // configure magnetometer, enable continuous measurement mode (8 Hz)
  i2c_reg_write(MAG_ADDRESS, AK8963_CNTL1, 0x02);
}

mpu9250_measurement_t mpu9250_read_accelerometer() {
  // read values
  int16_t x_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_XOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_XOUT_L);
  int16_t y_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_YOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_YOUT_L);
  int16_t z_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_ZOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_ACCEL_ZOUT_L);

  // convert to g
  // coversion at +/- 2 g is 16384 LSB/g
  mpu9250_measurement_t measurement = {0};
  measurement.x_axis = ((float)x_val) / 16384;
  measurement.y_axis = ((float)y_val) / 16384;
  measurement.z_axis = ((float)z_val) / 16384;
  return measurement;
}

mpu9250_measurement_t mpu9250_read_gyro() {
  // read values
  int16_t x_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_XOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_XOUT_L);
  int16_t y_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_YOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_YOUT_L);
  int16_t z_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_ZOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, MPU9250_GYRO_ZOUT_L);

  // convert to g
  // coversion at +/- 2000 degrees/second is 16.4 LSB/g
  mpu9250_measurement_t measurement = {0};
  measurement.x_axis = ((float)x_val) / 16.4;
  measurement.y_axis = ((float)y_val) / 16.4;
  measurement.z_axis = ((float)z_val) / 16.4;
  return measurement;
}

mpu9250_measurement_t mpu9250_read_magnetometer() {

  // read data
  // must read 8 bytes starting at the first status register
  uint8_t reg_addr = AK8963_ST1;
  uint8_t rx_buf[8] = {0};
  nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TXRX(MAG_ADDRESS,
      &reg_addr, 1, rx_buf, 8);
  ret_code_t error_code = nrfx_twim_xfer(i2c_instance, &xfer, 0);
  APP_ERROR_CHECK(error_code);

  // determine values
  int16_t x_val = (((uint16_t)rx_buf[2]) << 8) | rx_buf[1];
  int16_t y_val = (((uint16_t)rx_buf[4]) << 8) | rx_buf[3];
  int16_t z_val = (((uint16_t)rx_buf[6]) << 8) | rx_buf[5];

  // convert to g
  // coversion is 0.6 uT/LSB
  mpu9250_measurement_t measurement = {0};
  measurement.x_axis = ((float)x_val) * 0.6;
  measurement.y_axis = ((float)y_val) * 0.6;
  measurement.z_axis = ((float)z_val) * 0.6;

  return measurement;
}

