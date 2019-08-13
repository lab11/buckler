/*

	Original Author:		Jeff C. Jensen
    Rewritten by: Joshua Adkins, Neal Jackson
	Revised:	2018-08-06
*/

#include <stdlib.h>

#include "app_error.h"

#include "kobukiSensor.h"
#include "kobukiSensorPoll.h"


// Request sensor data and wait for response
int32_t kobukiSensorPoll(KobukiSensors_t* const	sensors){

	int32_t status = 0;

	// initialize communications buffer
    // We know that the maximum size of the packet is less than 140 based on documentation
	uint8_t packet[140] = {0};

	status = kobukiReadFeedbackPacket(packet, 140);

    if (status != NRF_SUCCESS) {
        return status;
    }

	// parse response
    kobukiParseSensorPacket(packet, sensors);

	return status;
}


