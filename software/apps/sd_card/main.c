// SD card app
//
// Writes sample lines to the SD card on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_sdh.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "simple_logger.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  printf("Started SD card demo app...\n");

  // Enable SoftDevice (used to get RTC running)
  nrf_sdh_enable_request();

  // Initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);

  // Configure GPIOs
  nrf_gpio_cfg_output(BUCKLER_SD_ENABLE);
  nrf_gpio_cfg_output(BUCKLER_SD_CS);
  nrf_gpio_cfg_output(BUCKLER_SD_MOSI);
  nrf_gpio_cfg_output(BUCKLER_SD_SCLK);
  nrf_gpio_cfg_input(BUCKLER_SD_MISO, NRF_GPIO_PIN_NOPULL);

  nrf_gpio_pin_set(BUCKLER_SD_ENABLE);
  nrf_gpio_pin_set(BUCKLER_SD_CS);

  // Initialize SD card
  const char filename[] = "testfile.log";
  const char permissions[] = "a"; // w = write, a = append

  // Start file
  simple_logger_init(filename, permissions);

  // If no header, add it
  simple_logger_log_header("HEADER for file \'%s\', written on %s \n", filename, "DATE");
  printf("Wrote header to SD card\n");

  // Write sample lines
  int nr_lines = 5;

  for (int i = 0; i < nr_lines; i++) {
    simple_logger_log("%s: Line %i - Additional line added\n", "TIMESTAMP", i+1);
    printf("Wrote line to SD card\n");
  }

  // Signal that lines were written
  nrf_gpio_cfg_output(BUCKLER_LED0);
  nrf_gpio_pin_clear(BUCKLER_LED0);
}
