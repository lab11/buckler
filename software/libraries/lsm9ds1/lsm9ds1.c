
#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_twi_mngr.h"

#include "lsm9ds1.h"

static const nrf_twi_mngr_t* i2c_manager = NULL;

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);
  return rx_buf;
}

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = {reg_addr, data};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, buf, 2, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
  APP_ERROR_CHECK(error_code);
}

// initialization and configuration
void lsm9ds1_init(const nrf_twi_mngr_t* i2c) {
  i2c_manager = i2c;

}

uint8_t lsm9ds1_whoami_acc() {
  return i2c_reg_read(0x6a, WHO_AM_I_XG);
}

uint8_t lsm9ds1_whoami_mag() {
  return i2c_reg_read(0x1C, WHO_AM_I_M);
}
