#include <math.h>
#include <stdio.h>
#include "controller.h"
#include "kobukiSensorTypes.h"
#include "display.h"

// configure initial state
KobukiSensors_t sensors = {0};
uint16_t previous_encoder = 0;
float distance = 0;

static float measure_distance(uint16_t current_encoder, uint16_t previous_encoder) {
  const float CONVERSION = 0.00065;

  float result = 0.0;
  if (current_encoder >= previous_encoder) {
    result = (float)current_encoder - (float)previous_encoder;
  } else {
    result = (float)current_encoder + (0xFFFF - (float)previous_encoder);
  }
  return result = result * CONVERSION;
}

robot_state_t controller(robot_state_t state) {
  // read sensors from robot
  kobukiSensorPoll(&sensors);

  // delay before continuing
  // Note: removing this delay will make responses quicker, but will result
  //  in printf's in this loop breaking JTAG
  //nrf_delay_ms(1);

  char buf[16];
  snprintf(buf, 16, "%f", distance);
  display_write(buf, DISPLAY_LINE_1);

  // handle states
  switch(state) {
    case OFF: {
      // transition logic
      if (is_button_pressed(&sensors)) {
        state = DRIVING;
        previous_encoder = sensors.leftWheelEncoder;
      } else {
        // perform state-specific actions here
        display_write("OFF", DISPLAY_LINE_0);
        kobukiDriveDirect(0,0);
        state = OFF;
      }
      break; // each case needs to end with break!
    }

    case DRIVING: {
      // transition logic
      if (is_button_pressed(&sensors)) {
        state = OFF;
      } else if (distance >= 1) {
        state = OFF;
      } else {
        // perform state-specific actions here
        display_write("DRIVING", DISPLAY_LINE_0);
        kobukiDriveDirect(100,100);
        uint16_t current_encoder = sensors.leftWheelEncoder;
        distance = measure_distance(current_encoder, previous_encoder);
        state = DRIVING;
      }
      break; // each case needs to end with break!
    }
  }

  // add other cases here
  return state;
}