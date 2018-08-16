// Blink app
//
// Blinks the LEDs on Buckler

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

KobukiState_t state;
KobukiSensors_t initial_sensors;
KobukiSensors_t sensors;
static uint8_t i = 0;

int main(void) {
    kobukiInit();

    // initialize RTT library
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();
    printf("Initialized RTT!\n");

    while(1) {
        switch(state) {
        case DRIVING:
            kobukiDriveDirect(100,100);

            if(i >= 200) {
                state = TURNING;
                printf("Polling sensors\n");
                kobukiSensorPoll(&initial_sensors);
                printf("P: %d\n",initial_sensors.angle);
            } else {
                i++;
            }

            break;
        case TURNING:
            kobukiDriveRadius(150,100);
            kobukiSensorPoll(&sensors);

            if(abs(sensors.angle - initial_sensors.angle) >= 8500) {
                state = DRIVING;
                printf("Driving!\n");
                i = 0;
            }

            break;
        };
        nrf_delay_ms(10);
    }
}

