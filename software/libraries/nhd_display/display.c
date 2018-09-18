// Display driver for the NHD-0216KZW
//
// Write to the display

#include <string.h>

#include "nrf_delay.h"
#include "nrf_drv_spi.h"
#include "app_error.h"

#include "display.h"

static nrf_drv_spi_t* spi_instance;

ret_code_t display_init(nrf_drv_spi_t* spi) {
  spi_instance = spi;

  uint8_t write[2];

  // Set function 8 bit mode
  write[0] = 0b00001110;
  write[1] = 0b00000000;
  ret_code_t err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Turn display off
  write[0] = 0b00000010;
  write[1] = 0b00000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Clear display
  write[0] = 0b00000000;
  write[1] = 0b01000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Set entry mode to increment right no shift
  write[0] = 0b00000001;
  write[1] = 0b10000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Move cursor home
  write[0] = 0b00000000;
  write[1] = 0b10000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Move cursor home
  write[0] = 0b00000011;
  write[1] = 0b01000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  // Read the status bit
  write[0] = 0b01000000;
  write[1] = 0b00000000;
  err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
  APP_ERROR_CHECK(err_code);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  nrf_delay_ms(10);

  return NRF_SUCCESS;
}

ret_code_t display_write(const char* string, uint8_t row) {

  uint32_t len = strlen(string);
  if (len > 16) {
    return NRF_ERROR_INVALID_LENGTH;
  }

  // Set up a spi transfer struct
  uint8_t write[2];

  ret_code_t err_code;
  if (row == 0) {
    // Set the screen to the correct character (0)
    write[0] = 0b00100000;
    write[1] = 0b00000000;
    err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
    APP_ERROR_CHECK(err_code);
    if(err_code != NRF_SUCCESS) return err_code;

  } else if (row == 1) {
    // Set the screen to the correct character (0x40)
    write[0] = 0b00110000;
    write[1] = 0b00000000;
    err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
    APP_ERROR_CHECK(err_code);
    if(err_code != NRF_SUCCESS) return err_code;
  } else {
    return NRF_ERROR_INVALID_DATA;
  }

  // Now write the characters of the string then clear the line
  uint8_t base_char[2];
  base_char[0] = 0b10000000;
  base_char[1] = 0b00000000;
  for (uint8_t i = 0; i < 16; i++) {
    char to_write;

    if (i >= len) {
      //write spaces as padding to clear that row
      to_write = ' ';
    } else {
      to_write = string[i];
    }

    // write the character
    // the top 6 bits then the bottom two bits
    write[0] = base_char[0] | (to_write >> 2);
    write[1] = base_char[1] | (to_write << 6);
    err_code = nrf_drv_spi_transfer(spi_instance, write, 2, NULL, 0);
    APP_ERROR_CHECK(err_code);
    if (err_code != NRF_SUCCESS) {
      return err_code;
    }
  }

  return NRF_SUCCESS;
}

