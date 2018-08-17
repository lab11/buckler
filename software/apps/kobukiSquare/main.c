// Kobuki square app
//
// Drive the Kobuki robot in a square using the internal gyroscope

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

#include "buckler.h"
#include "kobukiActuator.h"
#include "kobukiSensorTypes.h"
#include "kobukiSensorPoll.h"
#include "kobukiUtilities.h"

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
  KobukiSensors_t initial_sensors;
  kobukiSensorPoll(&initial_sensors);

  // loop forever
  uint8_t i = 0;
  while (1) {

    // test current state
    switch (state) {
      case DRIVING: {
        kobukiDriveDirect(100,100);

        // continue driving until 200*10 = 2000 ms have passed
        if (i >= 200) {
          // transition to turning state
          state = TURNING;
          printf("Beginning turn. Polling sensors\n");
          kobukiSensorPoll(&initial_sensors);
          printf("Starting Angle: %d\n", initial_sensors.angle);
        } else {
          // continue driving
          i++;
        }

        break;
      }

      case TURNING: {
        kobukiDriveRadius(150,100);

        // check angle to see if we've reached more than 85 degrees
        KobukiSensors_t sensors;
        kobukiSensorPoll(&sensors);
        if (abs(sensors.angle - initial_sensors.angle) >= 8500) {
          // transition to driving state
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

