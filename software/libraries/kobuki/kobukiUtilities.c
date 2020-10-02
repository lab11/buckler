#include "kobukiUtilities.h"
#include <stdint.h>
#include <stdio.h>

#include "nrf_drv_clock.h"
#include "nrf_uarte.h"
#include "nrf_serial.h"
#include "app_timer.h"

#include "buckler.h"

#include "kobukiSensorTypes.h"

NRF_SERIAL_DRV_UART_CONFIG_DEF(m_uart0_drv_config,
                      BUCKLER_UART_RX, BUCKLER_UART_TX,
                      0, 0,
                      NRF_UART_HWFC_DISABLED, NRF_UART_PARITY_EXCLUDED,
                      NRF_UART_BAUDRATE_115200,
                      UART_DEFAULT_CONFIG_IRQ_PRIORITY);

#define SERIAL_FIFO_TX_SIZE 512
#define SERIAL_FIFO_RX_SIZE 512

NRF_SERIAL_QUEUES_DEF(serial_queues, SERIAL_FIFO_TX_SIZE, SERIAL_FIFO_RX_SIZE);


#define SERIAL_BUFF_TX_SIZE 1
#define SERIAL_BUFF_RX_SIZE 1

NRF_SERIAL_BUFFERS_DEF(serial_buffs, SERIAL_BUFF_TX_SIZE, SERIAL_BUFF_RX_SIZE);

NRF_SERIAL_CONFIG_DEF(serial_config, NRF_SERIAL_MODE_DMA,
                      &serial_queues, &serial_buffs, NULL, NULL);


NRF_SERIAL_UART_DEF(serial_uart, 0);

const nrf_serial_t * serial_ref = &serial_uart;

int kobukiUARTInit() {
  return nrf_serial_init(&serial_uart, &m_uart0_drv_config, &serial_config);
}

int kobukiUARTUnInit() {
  return nrf_serial_uninit(&serial_uart);
}

int kobukiInit() {
  uint32_t err_code;

  err_code = nrf_drv_clock_init();
  if (err_code != NRF_ERROR_MODULE_ALREADY_INITIALIZED){
    APP_ERROR_CHECK(err_code);
  }

  nrf_drv_clock_lfclk_request(NULL);
  err_code = app_timer_init();
  if (err_code != NRF_ERROR_MODULE_ALREADY_INITIALIZED){
    APP_ERROR_CHECK(err_code);
  }

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

// checks for the state change of a button press on any of the Kobuki buttons
bool is_button_pressed(KobukiSensors_t* sensors) {
  // save previous states of buttons
  static bool previous_B0 = false;
  static bool previous_B1 = false;
  static bool previous_B2 = false;

  bool result = false;

  // check B0
  bool current_B0 = sensors->buttons.B0;
  if (current_B0 && previous_B0 != current_B0) {
    result = true;
  }
  previous_B0 = current_B0;

  // check B1
  bool current_B1 = sensors->buttons.B1;
  if (current_B1 && previous_B1 != current_B1) {
    result = true;
  }
  previous_B1 = current_B1;

  // check B2
  bool current_B2 = sensors->buttons.B2;
  if (current_B2 && previous_B2 != current_B2) {
    result = true;
  }
  previous_B2 = current_B2;

  return result;
}

