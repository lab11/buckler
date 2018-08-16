/*
	Original Author:		Jeff C. Jensen
    Rewritten By: Joshua Adkins
	Revised:	2018-08-06
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <app_uart.h>

#include "kobukiUART.h"
#include "kobukiUtilities.h"


int32_t kobukiReadFeedbackPacket(uint8_t* packetBuffer){

	typedef enum {
        wait_until_AA,
        wait_until_55,
        read_length,
        read_payload,
        read_checksum
    } state_type;

	state_type state = wait_until_AA;

	uint8_t byteBuffer;
	int status = 0;
	uint8_t payloadSize = 0;
	uint8_t calcuatedCS;

	status = app_uart_flush();

	int num_checksum_failures = 0;
    uint8_t p_index = 0;

	while(1){
		switch(state){
			case wait_until_AA:
                status = app_uart_get(&packetBuffer[p_index]);
                if(status == NRF_ERROR_NOT_FOUND) {
                    //there is no byte so just wait until there is
                    break;
                } else if(status != NRF_SUCCESS) {
                    return status;
                } else {
                    p_index++;
                }

				if (p_index == 2 && packetBuffer[0]==0xAA && packetBuffer[1]==0x55) {
					state = read_length;
                } else if (p_index == 2) {
                    p_index = 0;
					state = wait_until_AA;
                } else {
					state = wait_until_AA;
                }

				break;

			case read_length:
                status = app_uart_get(&packetBuffer[p_index]);
                if(status == NRF_ERROR_NOT_FOUND) {
                    //there is no byte so just wait until there is
                    break;
                } else if(status != NRF_SUCCESS) {
                    return status;
                }

                payloadSize = packetBuffer[p_index];
                byteBuffer = payloadSize;

                p_index++;
				state = read_payload;
				break;

			case read_payload:
                status = app_uart_get(&packetBuffer[p_index]);
                if(status == NRF_ERROR_NOT_FOUND) {
                    //there is no byte so just wait until there is
                    break;
                } else if(status != NRF_SUCCESS) {
                    return status;
                } else {
                    p_index++;
                }

                if(p_index > payloadSize+3) {
				    state = read_checksum;
                }

				break;

			case read_checksum:
				calcuatedCS = checkSumRead(packetBuffer, payloadSize + 3);
				byteBuffer=(packetBuffer)[payloadSize+3];
				if (calcuatedCS == byteBuffer) {
					num_checksum_failures = 0;
					return NRF_SUCCESS;
				} else{
					state = wait_until_AA;
					if (num_checksum_failures == 3) {
						return -1500;
					}
					num_checksum_failures++;
				}

				break;

            default:
                break;
		}

	}
	return status;
}
