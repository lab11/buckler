// Blink app
//
// Blinks an LED

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

void set_ble_payload(uint8_t* buffer, uint8_t length) {
    // TODO: implement this function!
}

void light_timer_callback() {
    // TODO: implement this function!
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

  // Set up an advertisement
  uint8_t message[24];
  // Send very recognizable data
  for (uint8_t i = 0; i < 24; i ++) {
    message[i] = i;
  }
  static uint8_t payload[24] = {0}; // 24 bytes are available for arbitrary data
  memcpy(payload, message, sizeof(message));

  static ble_advdata_manuf_data_t adv_payload = {
    .company_identifier = 0x02E0, // Lab11 company ID (University of Michigan)
    .data.p_data = payload,
    .data.size = sizeof(payload),
  };

  // create an advertisement with a manufacturer-specific data payload
  // https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Fstructble__advdata__t.html
  ble_advdata_t advdata = {0};
  advdata.name_type = BLE_ADVDATA_NO_NAME; // do not include device name (adv_name) in advertisement
  advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; // BLE Low energy advertisement
  advdata.p_manuf_specific_data = &adv_payload;

  // update advertisement data and start advertising
  simple_ble_set_adv(&advdata, NULL);

  // Set a timer to read the light sensor and update advertisement data.
  app_timer_init();
  app_timer_create(&adv_timer, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t) light_timer_callback);
  app_timer_start(adv_timer, APP_TIMER_TICKS(1000), NULL); // 1000 milliseconds

  while(1) {
    // Sleep while SoftDevice handles BLE
    power_manage();
  }
}

