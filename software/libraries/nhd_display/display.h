// Display driver for the NHD-0216KZW
//
// Write to the display

#pragma once

#include "nrf_drv_spi.h"
#include "app_error.h"

// Initialize the display
//
// Returns success or an error code
ret_code_t display_init(nrf_drv_spi_t* spi);

// Write to the display
// String is a null terminated c string with max length of 16 characters
// Row may either be set to 0 or 1
// Returns success or an error code
ret_code_t display_write(char* string, uint8_t row);
