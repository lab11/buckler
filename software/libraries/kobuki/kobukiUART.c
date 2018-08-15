/*
	Original Author:		Jeff C. Jensen
    Rewritten By: Joshua Adkins
	Revised:	2018-08-06
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nrf_serial.h"

#include "kobukiUART.h"
#include "kobukiUtilities.h"

extern nrf_serial_t serial_uart_instance;

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

	status = nrf_serial_rx_drain(&serial_uart_instance);

	int num_checksum_failures = 0;

	while(1){

		switch(state){
			case wait_until_AA:
                for(uint8_t i = 0; i < 2; i++) {
                    status = nrf_serial_read(&serial_uart_instance, (&packetBuffer[i]), 1, 0, 10);
                }

				if (status < NRF_SUCCESS) {
                    return status;
                }

				if (packetBuffer[0]==0xAA && packetBuffer[1]==0x55) {
					state=read_length;
                } else {
					state = wait_until_AA;
                }

				break;

			case read_length:
				byteBuffer = 0;
                status = nrf_serial_read(&serial_uart_instance, &byteBuffer, 1, 0, 10);
                if(status < NRF_SUCCESS) {
                    return status;
                }

				payloadSize = byteBuffer;
                packetBuffer[2] = payloadSize;
				state = read_payload;
				break;

			case read_payload:
                for(uint8_t i = 0; i < payloadSize+1; i++) {
                    status = nrf_serial_read(&serial_uart_instance, &packetBuffer[3+i], 1, 0, 10);
                    if(status < NRF_SUCCESS) {
                        return status;
                    }
                }

				if (status < NRF_SUCCESS) {
                    return status;
				}

				state = read_checksum;
				break;

			case read_checksum:
				calcuatedCS = checkSumRead(packetBuffer, payloadSize + 3);
				byteBuffer=(packetBuffer)[payloadSize+3];
				if (calcuatedCS == byteBuffer) {
					num_checksum_failures = 0;
					return status;
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
