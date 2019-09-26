// BLE advertisement template
//
// Advertises device name: EE149

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_twi_mngr.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "buckler.h"
#include "simple_ble.h"

#include "max44009.h"

// Create a timer
APP_TIMER_DEF(adv_timer);

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000,  // TODO: replace with your lab bench number
        .adv_name          = "EE149", // Note that this name is not displayed to save room in the advertisement for data.
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

void light_timer_callback() {
    printf("Light timer fired!\n");
    // TODO: implement this function!
    // Use Simple BLE function to read light sensor and put data in
    // advertisement, but be careful about doing it in the callback itself!
}

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // Initialize

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize MAX44009 driver
  const max44009_config_t config = {
    .continuous = 0,
    .manual = 0,
    .cdr = 0,
    .int_time = 3,
  };
  max44009_init(&twi_mngr_instance, BUCKLER_LIGHT_INTERRUPT);
  max44009_config(config);
  printf("MAX44009 initialized\n");

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  // TODO replace this with advertisement sending light data
  simple_ble_adv_only_name();

  // Set a timer to read the light sensor and update advertisement data every second.
  app_timer_init();
  app_timer_create(&adv_timer, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t) light_timer_callback);
  app_timer_start(adv_timer, APP_TIMER_TICKS(1000), NULL); // 1000 milliseconds

  while(1) {
    // Sleep while SoftDevice handles BLE
    power_manage();
  }
}

