// Kobuki sensors app
//
// Read and print sensor values from the Kobuki robot

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

int main(void) {

  // intitialize Kobuki library
  kobukiInit();

  // initialize RTT library
  NRF_LOG_INIT(NULL);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Initialized RTT!\n");

  // loop forever
  while (1) {

    // read kobuki sensor values
    KobukiSensors_t sensors;
    kobukiSensorPoll(&sensors);

    // print results
    printf("\n");
    printf("### Kobuki Sensor Values ###\n");
    printf("Wheel drop left:\t%d\n", sensors.bumps_wheelDrops.wheeldropLeft);
    printf("Wheel drop right:\t%d\n", sensors.bumps_wheelDrops.wheeldropRight);
    printf("\n");
    printf("Bump left:\t%d\n", sensors.bumps_wheelDrops.bumpLeft);
    printf("Bump center:\t%d\n", sensors.bumps_wheelDrops.bumpCenter);
    printf("Bump right:\t%d\n", sensors.bumps_wheelDrops.bumpRight);
    printf("\n");
    printf("Cliff left:\t%d\n", sensors.cliffLeft);
    printf("Cliff center:\t%d\n", sensors.cliffCenter);
    printf("Cliff right:\t%d\n", sensors.cliffRight);
    printf("\n");
    printf("Button 0:\t%d\n", sensors.buttons.B0);
    printf("Button 1:\t%d\n", sensors.buttons.B1);
    printf("Button 2:\t%d\n", sensors.buttons.B2);
    printf("\n");
    printf("Encoder left:\t%d\n", sensors.leftWheelEncoder);
    printf("Encoder right\t%d\n", sensors.rightWheelEncoder);
    printf("\n");
    printf("Current left:\t%d\n", sensors.leftWheelCurrent);
    printf("Current right\t%d\n", sensors.rightWheelCurrent);
    printf("\n");
    printf("PWM left:\t%d\n", sensors.leftWheelPWM);
    printf("PWM right\t%d\n", sensors.rightWheelPWM);
    printf("\n");
    printf("Over current left:\t%d\n", sensors.leftWheelOverCurrent);
    printf("Over current right\t%d\n", sensors.rightWheelOverCurrent);
    printf("\n");
    printf("Battery voltage:\t%d\n", sensors.batteryVoltage);
    printf("Charging state:\t%d\n", sensors.chargingState);
    printf("\n");
    printf("Angle:\t%d\n", sensors.angle);
    printf("Rate:\t%d\n", sensors.angleRate);

    nrf_delay_ms(500);
  }
}

