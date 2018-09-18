// Kobuki Square IMU
//
// Drives in a square using the external IMU

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_twi_mngr.h"

#include "buckler.h"

#include "kobukiActuator.h"
#include "kobukiSensorTypes.h"
#include "kobukiSensorPoll.h"
#include "kobukiUtilities.h"

#include "mpu9250.h"

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

typedef enum {
  DRIVING,
  TURNING
} KobukiState_t;

int main(void) {

  // initialize Kobuki library
  kobukiInit();

  // initialize RTT library
  NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Initialized RTT!\n");

  // initialize state
  KobukiState_t state = DRIVING;

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  ret_code_t error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize MPU-9250 driver
  mpu9250_init(&twi_mngr_instance);
  printf("MPU-9250 initialized\n");

  // loop forever
  static uint8_t i = 0;
  while(1) {

    // test current state
    switch(state) {
      case DRIVING: {
        kobukiDriveDirect(100,100);

        // continue driving until 200*10 = 2000 ms have passed
        if (i >= 200) {
          state = TURNING;
          printf("Beginning turn. Reading sensors\n");
          mpu9250_start_gyro_integration();
        } else {
          // continue driving
          i++;
        }

        break;
      }
      case TURNING: {
        kobukiDriveRadius(150,100);

        // check angle to see if we've reached more than 85 degrees
        mpu9250_measurement_t angle = mpu9250_read_gyro_integration();
        if (abs(angle.z_axis) >= 85) {
          // transition to driving state
          mpu9250_stop_gyro_integration();
          state = DRIVING;
          printf("Driving!\n");
          i = 0;
        }

        break;
      }
    };

    // continue for 10 ms before checking state again
    nrf_delay_ms(10);
  }
}

