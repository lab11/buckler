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



uint16_t read_encoder(){
	return sensors.leftWheelEncoder;
}

float update_dist(float dist, uint16_t prev_encoder, bool is_forward){
	const float CONVERSION = 0.00008529;
	uint16_t current_encoder = read_encoder();
	float result = 0.0;
	if (!is_forward){
		uint16_t temp = current_encoder;
		current_encoder = prev_encoder;
		prev_encoder = temp;
	}
  	if (current_encoder >= prev_encoder) {
    	// normal case
    	result = (float)current_encoder - (float)prev_encoder;
  	} else {
    	// account for wrap
    	result = (float)current_encoder + (0xFFFF - (float)prev_encoder);
  	}
  	result = result * CONVERSION;
	if (result> 1.0 || result< -1.0){
			return dist;
	}else{
			return dist +result;
	}
}

void drive_kobuki(bool is_forward){
    // Your code here
}

void stop_kobuki(){
    // Your code here
}

void turn_kobuki(bool is_clockwise){
    // Your code here
}

bool is_left_bumper(){
	return sensors.bumps_wheelDrops.bumpLeft;
}

bool is_right_bumper(){
	return sensors.bumps_wheelDrops.bumpRight;
}

bool is_front_bumper(){
	return sensors.bumps_wheelDrops.bumpCenter;
}

bool is_button_press(){
	return is_button_pressed(&sensors);
}

void start_gyro(){
	mpu9250_start_gyro_integration();
}

void stop_gyro(){
	mpu9250_stop_gyro_integration();
}

float read_gyro(){
	return mpu9250_read_gyro_integration().z_axis;
}

void print_angle(float angle){
	char buf[16];
	snprintf(buf, 16, "%f", angle);
	display_write(buf, DISPLAY_LINE_1);
}

void print_dist(float dist){
	char buf[16];
		snprintf(buf, 16, "%f", dist);
		display_write(buf, DISPLAY_LINE_1);
}

void print_state(states current_state){
	switch(current_state){
	case OFF:
		display_write("OFF", DISPLAY_LINE_0);
		break;
	case DRIVING:
		display_write("DRIVING", DISPLAY_LINE_0);
		break;
	case BACKUP:
		display_write("BACKUP", DISPLAY_LINE_0);
		break;
	case RIGHT:
		display_write("RIGHT", DISPLAY_LINE_0);
		break;
	case LEFT:
			display_write("LEFT", DISPLAY_LINE_0);
			break;
	case REORIENT:
			display_write("REORIENT", DISPLAY_LINE_0);
			break;
	}
    // Add more cases if you need to print more states
}

void print_turn(turns current_turn){
	switch(current_turn){
	case RIGHT_TURN:
		display_write("RIGHT TURN", DISPLAY_LINE_0);
		break;
	case LEFT_TURN:
			display_write("LEFT TURN", DISPLAY_LINE_0);
			break;
	case STRAIGHT:
			display_write("STRAIGHT", DISPLAY_LINE_0);
			break;
	}
}
