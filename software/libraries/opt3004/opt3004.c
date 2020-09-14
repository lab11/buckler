#include <math.h>

#include "opt3004.h"
#include "buckler.h"

const nrf_twi_mngr_t* twi_mngr;

uint16_t opt3004_read_reg(uint8_t i2c_addr, uint8_t reg_addr) {
  uint16_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 2, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(twi_mngr, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);

  uint8_t temp = rx_buf & 0xFF;
  rx_buf >>= 8;
  rx_buf |= (temp << 8);
  return rx_buf;
}

static void opt3004_write_reg(uint8_t i2c_addr, uint8_t reg_addr, uint16_t data) {
  uint8_t * data8 = (uint8_t*) &data;
  uint8_t buf[3] = {reg_addr, data8[1], data8[0]};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, buf, 3, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(twi_mngr, NULL, write_transfer, 1, NULL);
  APP_ERROR_CHECK(error_code);
}

void opt3004_init(const nrf_twi_mngr_t* twi) {
  twi_mngr = twi;
}

ret_code_t opt3004_config(opt3004_config_t config) {
  uint16_t ctrl_reg = 0;

  // check that range number is less than 0x0C
  if (config.range_number > 0x0C) {
    return NRF_ERROR_INVALID_STATE;
  }

  ctrl_reg |= (config.range_number << OPT3004_CONFIG_RN_SHIFT);
  ctrl_reg |= (config.conversion_time << OPT3004_CONFIG_CT_SHIFT);
  ctrl_reg |= (config.latch_interrupt << OPT3004_CONFIG_L_SHIFT);
  ctrl_reg |= (config.interrupt_polarity << OPT3004_CONFIG_POL_SHIFT);
  ctrl_reg |= (config.fault_count << OPT3004_CONFIG_FC_SHIFT);

  opt3004_write_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG, ctrl_reg);

  return NRF_SUCCESS;
}

void opt3004_continuous() {
  uint16_t config_reg = opt3004_read_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG);
  printf("config_reg: %x\n", config_reg);
  config_reg &= ~(0x3 << OPT3004_CONFIG_M_SHIFT);
  printf("config_reg clear: %x\n", config_reg);
  config_reg |= (0x2 << OPT3004_CONFIG_M_SHIFT);
  printf("config_reg wrote: %x\n", config_reg);
  opt3004_write_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG, config_reg);
}

void opt3004_single_shot() {
  uint16_t config_reg = opt3004_read_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG);
  config_reg |= (0x1 << OPT3004_CONFIG_M_SHIFT);
  opt3004_write_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG, config_reg);
}

void opt3004_shutdown() {
  uint16_t config_reg = opt3004_read_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG);
  config_reg &= ~(0x11 << OPT3004_CONFIG_M_SHIFT);
  opt3004_write_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_RN_SHIFT, config_reg);
}

float opt3004_read_result() {
  // busy loop until result ready
  while(!(opt3004_read_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_CONFIG_REG) & OPT3004_CONFIG_CRF_MASK)) {}
  // read result register
  uint16_t result = opt3004_read_reg(BUCKLER_OPT3004_I2C_ADDR, OPT3004_RESULT_REG);

  // split result into exponent and fractional components
  uint16_t exp = (result & OPT3004_RESULT_E_MASK) >> OPT3004_RESULT_E_SHIFT;
  uint16_t frac = (result & OPT3004_RESULT_R_MASK) >> OPT3004_RESULT_R_SHIFT;
  float lsb = 0.01 * powf(2, exp);
  return lsb * frac;
}
