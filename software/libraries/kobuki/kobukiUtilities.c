#include "kobukiUtilities.h"
#include <stdint.h>
#include <stdio.h>

#include "app_uart.h"
#include "nrf_uarte.h"
#include "buckler.h"

static void uart_error_handle (app_uart_evt_t * p_event) {
    if (p_event->evt_type == APP_UART_COMMUNICATION_ERROR) {
        //APP_ERROR_HANDLER(p_event->data.error_communication);
        app_uart_flush();
    } else if (p_event->evt_type == APP_UART_FIFO_ERROR) {
        //APP_ERROR_HANDLER(p_event->data.error_code);
        app_uart_flush();
    }
}

int kobukiInit() {
  uint32_t err_code;

  const app_uart_comm_params_t comm_params =
  {
    BUCKLER_UART_RX,
    BUCKLER_UART_TX,
    0,
    0,
    APP_UART_FLOW_CONTROL_DISABLED,
    false,
    NRF_UARTE_BAUDRATE_115200
  };

  APP_UART_FIFO_INIT(&comm_params,
                     1024,
                     1024,
                     uart_error_handle,
                     APP_IRQ_PRIORITY_LOW,
                     err_code);

  APP_ERROR_CHECK(err_code);

  return err_code;
}

uint8_t checkSumRead(uint8_t * buffer, int length){
	int i;
    uint8_t cs = 0x00;
    //printf("\n l = %d \n",length);
    for(i = 2; i < length; i++ ){
    //	printf("%d ",buffer[i]);
    	 cs ^= buffer[i];
    }
   // printf("\n last[%d]=%d cs=%d",i,buffer[i],cs);
    return cs;
}
uint8_t checkSum(uint8_t * buffer, int length){
	int i;
    uint8_t cs = 0x00;
    //printf("\n l = %d \n",length);
    for(i = 2; i < length; i++ ){
    	//printf("%d ",buffer[i]);
    	 cs ^= buffer[i];
    }
    //printf("\n last[%d]=%d cs=%d",i,buffer[i],cs);
    return cs;
}
