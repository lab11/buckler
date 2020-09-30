/*
	Original Author:    Jeff C. Jensen
    Rewritten By:       Joshua Adkins, Neal Jackson
	Revised:	2018-08-06
*/

#include "kobukiActuator.h"
#include "kobukiUtilities.h"

#include "app_error.h"
#include "nrf_serial.h"
#include "nrf_delay.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

extern const nrf_serial_t * serial_ref;

static int32_t kobukiSendPayload(uint8_t* payload, uint8_t len) {
    uint8_t writeData[256] = {0};

    // Write move payload
    writeData[0] = 0xAA;
    writeData[1] = 0x55;
    writeData[2] = len;
    memcpy(writeData + 3, payload, len);
	writeData[3+len] = checkSum(writeData, 3 + len);
   
    int status = nrf_serial_write(serial_ref, writeData, len + 4, NULL, 10);
    if(status != NRF_SUCCESS) {
        return status;
    }
    return NRF_SUCCESS;
    

}

int32_t kobukiDriveDirect(int16_t leftWheelSpeed, int16_t rightWheelSpeed){
	int32_t CmdSpeed;
	int32_t CmdRadius;

	if (abs(rightWheelSpeed) > abs(leftWheelSpeed)) {
	    CmdSpeed = rightWheelSpeed;
	} else {
	    CmdSpeed = leftWheelSpeed;
	}

	if (rightWheelSpeed == leftWheelSpeed) {
	    CmdRadius = 0;  // Special case 0 commands Kobuki travel with infinite radius.
	} else {
	    CmdRadius = (rightWheelSpeed + leftWheelSpeed) / (2.0 * (rightWheelSpeed - leftWheelSpeed) / 123.0);  // The value 123 was determined experimentally to work, and is approximately 1/2 the wheelbase in mm.
	    CmdRadius = round(CmdRadius);
	    //if the above statement overflows a signed 16 bit value, set CmdRadius=0 for infinite radius.
	    if (CmdRadius>32767) CmdRadius=0;
	    if (CmdRadius<-32768) CmdRadius=0;
	    if (CmdRadius==0) CmdRadius=1;  // Avoid special case 0 unless want infinite radius.
	}

	if (CmdRadius == 1){
		CmdSpeed = CmdSpeed * -1;
	}

	int32_t status = kobukiDriveRadius(CmdRadius, CmdSpeed);


	return status;
}

int32_t kobukiDriveRadius(int16_t radius, int16_t speed){
    uint8_t payload[6];
    payload[0] = 0x01;
    payload[1] = 0x04;
    memcpy(payload+2, &speed, 2);
    memcpy(payload+4, &radius, 2);

    return kobukiSendPayload(payload, 6);
}

int32_t kobukiSetControllerDefault(void) {
    uint8_t payload[15] = {0};

    payload[0] = 0x0D; // PID type 13
    payload[1] = 0x0D; // 13 byte PID length
    payload[2] = 0x00; // Default gain

    return kobukiSendPayload(payload, 15);
}

int32_t kobukiSetControllerUser(uint32_t Kp, uint32_t Ki, uint32_t Kd){
    uint8_t payload[15] = {0};

    payload[0] = 0x0D; // PID type 13
    payload[1] = 0x0D; // 13 byte PID length
    payload[2] = 0x01; // User gain

    //The values passed in are multiplied by 1000 to be represented by integers
    // P = Kp/1000
    // I = Ki/1000;
    // D = Kd/1000;
    memcpy(payload + 3, &Kp, 4);
    memcpy(payload + 7, &Ki, 4);
    memcpy(payload + 11, &Kd, 4);

    return kobukiSendPayload(payload, 15);
}

// Play a sound of f = 1/(frequency * 0.00000275) with duration
/*int32_t kobukiPlaySound(uint32_t frequency_hz, uint8_t duration_ms) {

    uint16_t use_f = (uint16_t)(1.0/(frequency_hz * 0.00000275));

    uint8_t payload[5] = {0};

    payload[0] = 0x03;
    payload[1] = 0x03;
    memcpy(payload + 2, &use_f, 2);
    payload[4] = duration_ms;

    return kobukiSendPayload(payload, 5);
}*/

// Play a predefined sound from the above sound types
int32_t kobukiPlaySoundSequence(KobukiSound_t sound) {
    uint8_t payload[3] = {0};

    payload[0] = 0x04;
    payload[1] = 0x01;
    payload[2] = (uint8_t)sound;

    return kobukiSendPayload(payload, 3);
}

// Request hardware version, firmware version and unique ID on the next data packet
int32_t kobukiRequestInformation(void) {
    uint8_t payload[4] = {0};

    payload[0] = 0x09;
    payload[1] = 0x02;
    payload[2] = 0x08 | 0x02 | 0x01;
    payload[3] = 0x00;

    return kobukiSendPayload(payload, 4);
}
// Control Output and LEDs on the Robot
// The four least significant bits of outputs controls outputs 0-3
// The four least significant bits of leds controls leds 0-3
// All power channels are enabled by this command to prevent
// accidental power down of the controller board
/*int32_t kobukiGeneralOutput(uint8_t outputs, uint8_t leds) {
    uint8_t payload[4] = {0};

    uint16_t general_output = 0;
    //general_output = (leds & 0x0F << 8) | (0x0F << 4) | (outputs & 0x0F);

    payload[0] = 0x0C;
    payload[1] = 0x02;
    memcpy(payload + 2, &general_output, 2);

    return kobukiSendPayload(payload, 4);
}*/
