// Blink app
//
// Blinks the LEDs on Buckler

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"
#include "nrfx_twim.h"

#include "buckler.h"
#include "lsm9ds1.h"

// LED array
static uint8_t LEDS[3] = {BUCKLER_LED0, BUCKLER_LED1, BUCKLER_LED2};

// ADC channels
#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

// handler called whenever an input pin changes
void pin_change_handler(nrfx_gpiote_pin_t _pin, nrf_gpiote_polarity_t _action) {
  if (nrfx_gpiote_in_is_set(BUCKLER_SWITCH0)) {
    nrfx_gpiote_out_clear(LEDS[2]);
  } else {
    if (nrfx_gpiote_in_is_set(BUCKLER_BUTTON0)) {
      nrfx_gpiote_out_set(LEDS[2]);
    } else {
      nrfx_gpiote_out_clear(LEDS[2]);
    }
  }
}

// callback for SAADC events
void saadc_callback (nrfx_saadc_evt_t const * p_event) {
  // don't care about adc callbacks
}

// sample a particular analog channel in blocking mode
nrf_saadc_value_t sample_value (uint8_t channel) {
  nrf_saadc_value_t val;
  ret_code_t error_code = nrfx_saadc_sample_convert(channel, &val);
  APP_ERROR_CHECK(error_code);
  return val;
}

int main(void) {
  ret_code_t error_code = NRF_SUCCESS;

  // initialize GPIO driver
  if (!nrfx_gpiote_is_init()) {
    error_code = nrfx_gpiote_init();
  }
  APP_ERROR_CHECK(error_code);

  // configure leds
  // manually-controlled (simple) output, initially set
  nrfx_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(true);
  for (int i=0; i<3; i++) {
    error_code = nrfx_gpiote_out_init(LEDS[i], &out_config);
    APP_ERROR_CHECK(error_code);
  }

  // configure button and switch
  // input pin, trigger on either edge, low accuracy (allows low-power operation)
  nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
  in_config.pull = NRF_GPIO_PIN_NOPULL;
  error_code = nrfx_gpiote_in_init(BUCKLER_BUTTON0, &in_config, pin_change_handler);
  nrfx_gpiote_in_event_enable(BUCKLER_BUTTON0, true);
  in_config.pull = NRF_GPIO_PIN_NOPULL;
  error_code = nrfx_gpiote_in_init(BUCKLER_SWITCH0, &in_config, pin_change_handler);
  nrfx_gpiote_in_event_enable(BUCKLER_SWITCH0, true);
  pin_change_handler(0, 0);

  // initialize RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized!\n");

  // initialize analog to digital converter
  nrfx_saadc_config_t saadc_config = NRFX_SAADC_DEFAULT_CONFIG;
  saadc_config.resolution = NRF_SAADC_RESOLUTION_12BIT;
  error_code = nrfx_saadc_init(&saadc_config, saadc_callback);
  APP_ERROR_CHECK(error_code);

  // initialize analog inputs
  // configure with 0 as input pin for now
  nrf_saadc_channel_config_t channel_config = NRFX_SAADC_DEFAULT_CHANNEL_CONFIG_SE(0);
  channel_config.reference = NRF_SAADC_REFERENCE_INTERNAL; // 0.6 Volt reference
  channel_config.gain = NRF_SAADC_GAIN1_6; // multiply incoming signal by (1/6)

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_X;
  error_code = nrfx_saadc_channel_init(X_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Y;
  error_code = nrfx_saadc_channel_init(Y_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // specify input pin and initialize that ADC channel
  channel_config.pin_p = BUCKLER_ANALOG_ACCEL_Z;
  error_code = nrfx_saadc_channel_init(Z_CHANNEL, &channel_config);
  APP_ERROR_CHECK(error_code);

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = BUCKLER_SENSORS_SCL;
  i2c_config.sda = BUCKLER_SENSORS_SDA;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize MPU-9250 driver
  lsm9ds1_init(&twi_mngr_instance);
  printf("MPU-9250 initialized\n");


  // loop forever
  int loop_index = 0;
  while (1) {
    // blink two LEDs
    nrf_gpio_pin_toggle(LEDS[loop_index%2]);

    // sample analog inputs
    nrf_saadc_value_t x_val = sample_value(X_CHANNEL);
    nrf_saadc_value_t y_val = sample_value(Y_CHANNEL);
    nrf_saadc_value_t z_val = sample_value(Z_CHANNEL);

    // get imu measurements
    lsm9ds1_measurement_t acc_measurement = lsm9ds1_read_accelerometer();

    // print results
    printf("                        X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                    ----------\t----------\t----------\n");
    printf("Analog Accel (raw): %10d\t%10d\t%10d\n", x_val, y_val, z_val);
    printf("I2C IMU Accel (g):  %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("\n");

    // wait before continuing loop
    loop_index++;
    nrf_delay_ms(500);
  }
}

