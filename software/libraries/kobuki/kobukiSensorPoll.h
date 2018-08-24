/*
	Original Author:		Jeff C. Jensen
    Rewritten By: Joshua Adkins			
	Revised:	2018-08-06
*/

#ifndef _KOBUKI_SENSORPOLL_H
#define _KOBUKI_SENSORPOLL_H
#include <stdbool.h>
#include <stdint.h>

#include "kobukiUART.h"
#include "kobukiSensorTypes.h"
#include "kobukiSensor.h"

// Request sensor packet from kobuki and wait for response
int32_t kobukiSensorPoll(KobukiSensors_t * const	sensors);

#endif
