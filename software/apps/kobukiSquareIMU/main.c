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
#include "display.h"

#include "kobukiActuator.h"
#include "kobukiSensorTypes.h"
#include "kobukiSensorPoll.h"
#include "kobukiUtilities.h"

#include "lsm9ds1.h"

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

typedef enum {
  OFF,
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
  KobukiState_t state = OFF;
  KobukiSensors_t sensors = {0};

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_400K;
  ret_code_t error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize display
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };
  error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Hello, Human!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

  nrf_delay_ms(2000);

  // initialize LSM9DS1 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("lsm9ds1 initialized\n");

  // loop forever
  static uint8_t i = 0;
  static char display_buf[16];
  while(1) {
    // read sensors from robot
    kobukiSensorPoll(&sensors);

    // test current state
    switch(state) {
      case OFF: {
        kobukiDriveDirect(0,0);
        // transition logic
        if (is_button_pressed(&sensors)) {
          state = DRIVING;
        } else {
          // perform state-specific actions here
          display_write("OFF", DISPLAY_LINE_0);

          state = OFF;
        }
        break; // each case needs to end with break!
      }
      case DRIVING: {
        kobukiDriveDirect(75,75);

        // continue driving until 200*10 = 2000 ms have passed
        if (is_button_pressed(&sensors)) {
          state = OFF;
        }
        else if (i >= 200) {
          state = TURNING;
          printf("Beginning turn. Reading sensors\n");
          lsm9ds1_start_gyro_integration();

        } else {
          // continue driving
          display_write("DRIVING", DISPLAY_LINE_0);
          i++;
        }

        break;
      }
      case TURNING: {
        kobukiDriveDirect(75,-75);
        // check angle to see if we've reached more than 85 degrees
        lsm9ds1_measurement_t angle = lsm9ds1_read_gyro_integration();
        if (is_button_pressed(&sensors)) {
          state = OFF;
        }
        else if (abs(angle.z_axis) >= 85) {
          // transition to driving state
          lsm9ds1_stop_gyro_integration();
          state = DRIVING;
          printf("Driving!\n");
          i = 0;
        } else {
          snprintf(display_buf, 16, "TURNING: %.2f", angle.z_axis);
          display_write("TURNING: ", DISPLAY_LINE_0);
        }

        break;
      }
    };

    // continue for 10 ms before checking state again
    nrf_delay_ms(1);
  }
}

