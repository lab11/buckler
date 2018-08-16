/*

	Original Author:		Jeff C. Jensen
    Rewritten By: Joshua Adkins
	Revised:	2018-08-06
*/

#include "kobukiSensor.h"

#include <stdio.h>
#include <string.h>

static uint16_t to_uint16( uint8_t d1, uint16_t d2){
    return ( (uint16_t) d2 << 8) | d1 ;
}

void kobukiParseSensorPacket(const uint8_t * packet, KobukiSensors_t * sensors) {

	uint8_t payloadLength = packet[2];
	uint8_t i = 3;
	uint8_t subPayloadLength = 0;


	while( i < payloadLength + 3) {

		uint8_t idField = packet[i];

		subPayloadLength = packet[i+1];

		switch(idField) {
			case 0x01 :
				//There's an ambiguity in the documentation where
				//it says there are two headers with value 0x01:
				// basic sensor data and controller info - although it
				// says elsewhere that controller info has ID 0x15
				// so we'll just check here to make sure it's the right length

				if( subPayloadLength == 0x0F){
					sensors->timeStamp = to_uint16( packet[i+2], packet[i+3]);


					sensors->bumps_wheelDrops.bumpRight		=	packet[i+4] & 0x01;
					sensors->bumps_wheelDrops.bumpCenter	=	(packet[i+4] & 0x02);
					sensors->bumps_wheelDrops.bumpLeft		=	(packet[i+4] & 0x04);

					sensors->bumps_wheelDrops.wheeldropRight	= packet[i+5] & 0x01;
					sensors->bumps_wheelDrops.wheeldropLeft		=(packet[i+5] & 0x02);

					sensors->cliffRight		=	(packet[i+6] & 0x01);
					sensors->cliffCenter	=	(packet[i+6] & 0x02);
					sensors->cliffLeft		=	(packet[i+6] & 0x04);

					sensors->leftWheelEncoder	= to_uint16(packet[i+7], packet[i+8]);
					sensors->rightWheelEncoder = to_uint16(packet[i+9], packet[i+10]);

					sensors->leftWheelPWM		=	(int8_t) packet[i+11];
					sensors->rightWheelPWM 		=	(int8_t) packet[i+12];
					sensors->buttons.B0		=	(bool)(packet[i+13] & 0x01);
					sensors->buttons.B1		=	(bool)(packet[i+13] & 0x02);
					sensors->buttons.B2		=	(bool)(packet[i+13] & 0x04);

					// Charger state
					switch(packet[i+14]){
						case 0:
							sensors->chargingState = DISCHARGING;
							break;
						case 2:
							sensors->chargingState =  DOCKING_CHARGED;
							break;
						case 6:
							sensors->chargingState =  DOCKING_CHARGING;
							break;
						case 18:
							sensors->chargingState =  ADAPTER_CHARGED;
							break;
						case 22:
							sensors->chargingState =  ADAPTER_CHARGING;
							break;
					}

					sensors->batteryVoltage = packet[i+15];

					sensors->leftWheelOverCurrent = packet[i+16] & 0x01;
					sensors->rightWheelOverCurrent = packet[i+16] & 0x02;

					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}

				break;

			case 0x03 :
				if (subPayloadLength == 0x03){
                    sensors->docking.dockingRight = packet[i+2];
                    sensors->docking.dockingCenter = packet[i+3];
                    sensors->docking.dockingLeft = packet[i+4];
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}

				break;

			case 0x04 : // inertial sensor data
				if (subPayloadLength == 0x07){
                    sensors->angle = to_uint16(packet[i+2], packet[i+3]);
                    sensors->angleRate = to_uint16(packet[i+4], packet[i+5]);
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x05 : // cliff sensor data
				if (subPayloadLength == 0x06){
					sensors->cliffRightSignal=to_uint16(packet[i+2],packet[i+3]);
					sensors->cliffCenterSignal=to_uint16(packet[i+4],packet[i+5]);
					sensors->cliffLeftSignal=to_uint16(packet[i+6],packet[i+7]);
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x06 :
				if (subPayloadLength == 0x02){
                    sensors->leftWheelCurrent = to_uint16(packet[i+2], packet[i+3]);
                    sensors->rightWheelCurrent = to_uint16(packet[i+4], packet[i+5]);
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x0A :
				if (subPayloadLength == 0x04){
                    sensors->hardwareVersion.patch = packet[i+2];
                    sensors->hardwareVersion.minor = packet[i+3];
                    sensors->hardwareVersion.major = packet[i+4];
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x0B : // Firmware Version
				if (subPayloadLength == 0x04){
                    sensors->firmwareVersion.patch = packet[i+2];
                    sensors->firmwareVersion.minor = packet[i+3];
                    sensors->firmwareVersion.major = packet[i+4];
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x0D : // Raw 3d Gyro DAta
				if (subPayloadLength % 6 == 2){ // variable length packet. See documentation
                    sensors->xAxisRate = to_uint16(packet[i+5], packet[i+6]);
                    sensors->yAxisRate = to_uint16(packet[i+7], packet[i+8]);
                    sensors->zAxisRate = to_uint16(packet[i+9], packet[i+10]);
					i += subPayloadLength + 2; // + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x10 : //General purpose input
				if (subPayloadLength == 0x10){
                    sensors->generalInput.D0 = packet[i+2] & 0x01;
                    sensors->generalInput.D0 = packet[i+2] & 0x02;
                    sensors->generalInput.D0 = packet[i+2] & 0x04;
                    sensors->generalInput.D0 = packet[i+2] & 0x08;
                    sensors->generalInput.A0 = to_uint16(packet[i+4], packet[i+5]);
                    sensors->generalInput.A1 = to_uint16(packet[i+6], packet[i+7]);
                    sensors->generalInput.A2 = to_uint16(packet[i+8], packet[i+9]);
                    sensors->generalInput.A3 = to_uint16(packet[i+10], packet[i+11]);
					i += subPayloadLength + 2;	// + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x13 : // UID
				if (subPayloadLength == 0x0C){
                    memcpy(&sensors->UID[0], &packet[i+2], 4);
                    memcpy(&sensors->UID[1], &packet[i+6], 4);
                    memcpy(&sensors->UID[2], &packet[i+10], 4);
					i += subPayloadLength + 2;	// + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

			case 0x15 :
				if (subPayloadLength == 0x0D){
                    sensors->controllerGain.userConfigured = packet[i+2] == 0x01;
                    memcpy(&sensors->controllerGain.Kp, &packet[i+3], 4);
                    memcpy(&sensors->controllerGain.Ki, &packet[i+7], 4);
                    memcpy(&sensors->controllerGain.Kd, &packet[i+11], 4);
					i += subPayloadLength + 2;	// + 2 for header and length
				} else {
					i += payloadLength + 3; // add enough to terminate the outer while loop
				}
				break;

		}
	}

	//Checksum has already been checked.
	return;
}
