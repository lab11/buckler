/*	Project:	iRobot Create

	Author:		Jeff C. Jensen
				National Instruments

	Abstract:	Functions to read sensors from a data stream

	Revised:	2011-12-20
*/

#ifndef _KOBUKISENSOR_H
#define _KOBUKISENSOR_H
#include <stdbool.h>
#include <stdint.h>

#include "kobukiSensorTypes.h"
void kobukiParseSensorPacket(
	const uint8_t * packet,
	KobukiSensors_t * sensors
);

#endif

