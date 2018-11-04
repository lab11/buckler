// BLE TX app
//
// Sends BLE advertisements with data

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "simple_ble.h"

// Create a timer
APP_TIMER_DEF(adv_timer);

// BLE configuration
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:49:00:01
        .platform_id       = 0x49,    // used as 4th octet in device BLE address
        .device_id         = 0x0001,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "EE149", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;


// Sends the specified data over BLE advertisements
void set_ble_payload(uint8_t* buffer, uint8_t length) {
  static uint8_t adv_buffer[24] = {0};
  static ble_advdata_manuf_data_t adv_payload = {
    .company_identifier = 0x02E0, // Lab11 company ID (University of Michigan)
    .data.p_data = adv_buffer,
    .data.size = 24,
  };

  // copy over up to 23 bytes of advertisement payload
  adv_buffer[0] = 0x23; // identifies a Buckler advertisement payload
  if (length > 23) {
    length = 23; // maximum size is 23 bytes of payload
  }
  memcpy(&(adv_buffer[1]), buffer, length);
  adv_payload.data.size = 1+length;

  // create an advertisement with a manufacturer-specific data payload
  // https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v15.0.0%2Fstructble__advdata__t.html
  ble_advdata_t advdata = {0};
  advdata.name_type = BLE_ADVDATA_NO_NAME; // do not include device name (adv_name) in advertisement
  advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE; // BLE Low energy advertisement
  advdata.p_manuf_specific_data = &adv_payload;

  // update advertisement data and start advertising
  simple_ble_set_adv(&advdata, NULL);
}

// Callback when the timer fires. Updates the advertisement data
void adv_timer_callback(void) {
  static uint8_t increment = 0;

  // Update advertisement data
  // Increments each value by one each time
  increment++;
  uint8_t buffer[23] = {0};
  for (int i=0; i<23; i++) {
    buffer[i] = i+increment;
  }
  printf("Buffer value: %d\n", buffer[0]);
  set_ble_payload(buffer, 23);
}


int main(void) {

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);

  // Set advertisement data. Maximum size is 23 bytes of payload
  uint8_t buffer[23] = {0};
  for (int i=0; i<23; i++) {
    buffer[i] = i;
  }
  set_ble_payload(buffer, 23);

  // Set a timer to change the data. Data could also be changed after sensors
  // are read in real applications
  app_timer_init();
  app_timer_create(&adv_timer, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t)adv_timer_callback);
  app_timer_start(adv_timer, APP_TIMER_TICKS(1000), NULL); // 1000 milliseconds

  // go into low power mode
  while(1) {
    power_manage();
  }
}

