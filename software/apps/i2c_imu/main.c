// Button and Switch app
//
// Uses a button and a switch to control LEDs

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_twi_mngr.h"
#include "nrfx_gpiote.h"

#include "buckler.h"
#include "mpu9250.h"

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize MPU-9250 driver
  mpu9250_init(&twi_mngr_instance);
  printf("MPU-9250 initialized\n");

  // loop forever
  float x_rot = 0;
  float y_rot = 0;
  float z_rot = 0;
  while (1) {
    // get measurements
    mpu9250_measurement_t acc_measurement = mpu9250_read_accelerometer();
    mpu9250_measurement_t gyr_measurement = mpu9250_read_gyro();
    mpu9250_measurement_t mag_measurement = mpu9250_read_magnetometer();

    // determine rotation from gyro
    // gyros are messy, so only add value if it is of significant magnitude
    // note that we are dividing by 10 since we are measuring over a tenth of a second
    float x_rot_amount = gyr_measurement.x_axis/10;
    if (abs(x_rot_amount) > 0.5) {
      x_rot += x_rot_amount;
    }
    float y_rot_amount = gyr_measurement.y_axis/10;
    if (abs(y_rot_amount) > 0.5) {
      y_rot += y_rot_amount;
    }
    float z_rot_amount = gyr_measurement.z_axis/10;
    if (abs(z_rot_amount) > 0.5) {
      z_rot += z_rot_amount;
    }

    // print results
    printf("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                  ----------\t----------\t----------\n");
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("Angle  (degrees): %10.3f\t%10.3f\t%10.3f\n", x_rot, y_rot, z_rot);
    printf("Magnetism   (uT): %10.3f\t%10.3f\t%10.3f\n", mag_measurement.x_axis, mag_measurement.y_axis, mag_measurement.z_axis);
    printf("\n");

    nrf_delay_ms(100);
  }
}

