#pragma once

#include <stdint.h>

#include "app_error.h"
#include "nrf_twi_mngr.h"

#include "opt3004_registers.h"

typedef enum {
  OPT3004_CONVERSION_100MS = 0,
  OPT3004_CONVERSION_800MS = 1,
} opt3004_conversion_time_t;

typedef enum {
  OPT3004_INTERRUPT_ACTIVE_LO = 0,
  OPT3004_INTERRUPT_ACTIVE_HI = 1,
} opt3004_int_polarity_t;

typedef struct {
  uint8_t                   range_number;       // Sensor range. Use OPT3004_AUTORANGE to enable autoranging
  opt3004_conversion_time_t conversion_time;    // Conversion time of sensor. 100 or 800ms
  bool                      latch_interrupt;    // Latch interrupts until config register is read
  opt3004_int_polarity_t    interrupt_polarity; // Interrupt polarity. Active high or low
  uint8_t                   fault_count;        // Number of faults before interrupt trigger
} opt3004_config_t;


// provide twi_mngr instance to driver
// i2c: const pointer to nrf_twi_mngr_t instance
void opt3004_init(const nrf_twi_mngr_t* i2c);

// configure opt3004 with settings from config struct
// config: config struct
// returns error code
ret_code_t opt3004_config(opt3004_config_t config);

// Set opt3004 to sample continuously
void opt3004_continuous();

// Set opt3004 to sample a single measurement
void opt3004_single_shot();

// Set opt3004 to shutdown mode
void opt3004_shutdown();

// Read lux value from sensor
// returns floating point lux value
float opt3004_read_result();
