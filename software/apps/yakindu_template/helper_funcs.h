/*
 * helper_funcs.h
 *
 *  Created on: Sep 22, 2018
 *      Author: shromonaghosh
 */

#ifndef HELPER_FUNCS_H_
#define HELPER_FUNCS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "states.h"

extern void print_state(states current_state);
extern void print_turn(turns current_turn);
extern void print_dist(float dist);
extern void print_angle(float angle);

extern bool is_left_bumper();
extern bool is_right_bumper();
extern bool is_front_bumper();
extern bool is_button_press();

extern uint16_t read_encoder();
extern float update_dist(float dist, uint16_t prev_encoder, bool is_forward);

extern void drive_kobuki(bool is_forward);
extern void stop_kobuki();
extern void turn_kobuki(bool is_clockwise);

extern void start_gyro();
extern float read_gyro();
extern void stop_gyro();

#endif /* HELPER_FUNCS_H_ */
