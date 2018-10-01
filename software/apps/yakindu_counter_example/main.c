// yakindu app
//
// Run a simple yakindu statechart

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrfx_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"

#include "buckler.h"
#include "point.h"
#include "Test_geometry_sc.h"

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // start statechart
  Test_geometry_sc sc_handle;
  test_geometry_sc_init(&sc_handle);
  test_geometry_sc_enter(&sc_handle);

  // intialize statechart variables
  test_geometry_scIface_set_count(&sc_handle, 0);
  Point point = {
	.x = 0,
	.y = 0,
  };
  test_geometry_scIface_set_pointA(&sc_handle, point);

  // loop forever
  uint32_t i = 0;
  while (1) {
    // print status
    printf("Iteration: %lu\n", i++);
    printf("Count: %d\n", test_geometry_scIface_get_count(&sc_handle));
    Point newpoint = test_geometry_scIface_get_pointA(&sc_handle);
    printf("Point.x: %d\tPoint.y: %d\n", newpoint.x, newpoint.y);
    nrf_delay_ms(1000);

    // trigger a tick event on next `runCycle`
    test_geometry_scIface_raise_tick(&sc_handle);

    // iterate statechart
    test_geometry_sc_runCycle(&sc_handle);
  }
}

