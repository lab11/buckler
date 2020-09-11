#pragma once

#include <stdint.h>

#include "app_error.h"
#include "nrf_twi_mngr.h"

#include "opt3004_registers.h"

typedef struct {
  uint8_t range_number;
  bool    conversion_time;
  bool    latch_interrupt;
  bool    interrupt_polarity;
  uint8_t fault_count;
} opt3004_config_t;

void opt3004_init(const nrf_twi_mngr_t* i2c);

ret_code_t opt3004_config(opt3004_config_t config);

void opt3004_continuous();

void opt3004_single_shot();

void opt3004_shutdown();

float opt3004_read_result();
