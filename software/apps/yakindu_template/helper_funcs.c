/*
 * helper_funcs.c
 *
 *  Created on: Sep 23, 2018
 *      Author: shromonaghosh
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "helper_funcs.h"
#include "states.h"

#include "app_error.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_spi.h"

#include "buckler.h"
#include "display.h"
#include "kobukiActuator.h"
#include "kobukiSensorPoll.h"
#include "kobukiSensorTypes.h"
#include "kobukiUtilities.h"
#include "mpu9250.h"

extern KobukiSensors_t sensors;

uint16_t read_encoder() {
  return 0;
}

float update_dist(float dist, uint16_t prev_encoder, bool is_forward) {
  return 0.0;
}

void drive_kobuki(bool is_forward) {
}

void stop_kobuki() {
}

void turn_kobuki(bool is_clockwise) {
}

bool is_left_bumper() {
  return false;
}

bool is_right_bumper() {
  return false;
}

bool is_front_bumper() {
  return false;
}

bool is_button_press() {
  return false;
}

void start_gyro() {
}

void stop_gyro() {
}

float read_gyro() {
  return 0.0;
}

void print_angle(float angle) {
}

void print_dist(float dist) {
}

void print_state(states current_state) {
}

void print_turn(turns current_turn) {
}

