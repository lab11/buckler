// BLE Service Template
//
// Creates a service for changing LED state over BLE

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "app_util.h"
#include "nrf_twi_mngr.h"
#include "nrf_gpio.h"
#include "display.h"

#include "simple_ble.h"
#include "buckler.h"

#include "max44009.h"

// Create a timer
APP_TIMER_DEF(light_timer);

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
        // c0:98:e5:49:xx:xx
        .platform_id       = 0x49,    // used as 4th octect in device BLE address
        .device_id         = 0x0000, // TODO: replace with your lab bench number
        .adv_name          = "EE149 LED", // used in advertisements if there is room
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

// 32e61089-2b22-4db5-a914-43ce41986c70
static simple_ble_service_t led_service = {{
    .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
                0xB5,0x4D,0x22,0x2B,0x89,0x10,0xE6,0x32}
}};

static simple_ble_char_t led_state_char = {.uuid16 = 0x1090};
static bool led_state = true;

void light_timer_callback() {
    //TODO: implement this function to initiate a read of the light sensor
}

/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
    if (simple_ble_is_char_event(p_ble_evt, &led_state_char)) {
      printf("Got write to LED characteristic!\n");
      if (led_state) {
        printf("Turning on LED!\n");
        nrf_gpio_pin_clear(BUCKLER_LED0);
      } else {
        printf("Turning off LED!\n");
        nrf_gpio_pin_set(BUCKLER_LED0);
      }
    }
}

int main(void) {

  // Initialize

  // initialize display
  nrf_drv_spi_t spi_instance = NRF_DRV_SPI_INSTANCE(1);
  nrf_drv_spi_config_t spi_config = {
    .sck_pin = BUCKLER_LCD_SCLK,
    .mosi_pin = BUCKLER_LCD_MOSI,
    .miso_pin = BUCKLER_LCD_MISO,
    .ss_pin = BUCKLER_LCD_CS,
    .irq_priority = NRFX_SPI_DEFAULT_CONFIG_IRQ_PRIORITY,
    .orc = 0,
    .frequency = NRF_DRV_SPI_FREQ_4M,
    .mode = NRF_DRV_SPI_MODE_2,
    .bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
  };

  ret_code_t error_code = nrf_drv_spi_init(&spi_instance, &spi_config, NULL, NULL);
  APP_ERROR_CHECK(error_code);
  display_init(&spi_instance);
  display_write("Hello, Human!", DISPLAY_LINE_0);
  printf("Display initialized!\n");

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

  // Setup LED GPIO
  nrf_gpio_cfg_output(BUCKLER_LED0);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&led_service);

  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(led_state), (uint8_t*)&led_state,
      &led_service, &led_state_char);

  // Start Advertising
  simple_ble_adv_only_name();

  // Set a timer to read the light sensor and notify readers
  app_timer_init();
  app_timer_create(&light_timer, APP_TIMER_MODE_REPEATED, (app_timer_timeout_handler_t) light_timer_callback);
  app_timer_start(light_timer, APP_TIMER_TICKS(1000), NULL); // 1000 milliseconds

  while(1) {
    power_manage();
  }
}

