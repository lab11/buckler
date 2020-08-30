
#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_twi_mngr.h"

#include "lsm9ds1.h"

static IMUSettings settings;
static int16_t gx, gy, gz;
static int16_t ax, ay, az;
static int16_t mx, my, mz;
static bool autocalc;
static float gRes, aRes, mRes;
static float gBias[3], aBias[3], mBias[3];
static int16_t gBiasRaw[3], aBiasRaw[3], mBiasRaw[3];

static void i2c_read_bytes(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* data, uint8_t len) {
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, data, len, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(settings.device.i2c, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);
}

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(settings.device.i2c, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);
  return rx_buf;
}

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = {reg_addr, data};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, buf, 2, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(settings.device.i2c, NULL, write_transfer, 1, NULL);
  APP_ERROR_CHECK(error_code);
}

void calcgRes()
{
  switch (settings.gyro.scale)
  {
    case G_SCALE_245DPS:
      gRes = SENSITIVITY_GYROSCOPE_245;
      break;
    case G_SCALE_500DPS:
      gRes = SENSITIVITY_GYROSCOPE_500;
      break;
    case G_SCALE_2000DPS:
      gRes = SENSITIVITY_GYROSCOPE_2000;
      break;
    default:
      break;
  }
}

void calcaRes()
{
  switch (settings.accel.scale)
  {
    case A_SCALE_2G:
      aRes = SENSITIVITY_ACCELEROMETER_2;
      break;
    case A_SCALE_4G:
      aRes = SENSITIVITY_ACCELEROMETER_4;
      break;
    case A_SCALE_8G:
      aRes = SENSITIVITY_ACCELEROMETER_8;
      break;
    case A_SCALE_16G:
      aRes = SENSITIVITY_ACCELEROMETER_16;
      break;
    default:
      break;
  }
}

void calcmRes()
{
  switch (settings.mag.scale)
  {
    case M_SCALE_4GS:
      mRes = SENSITIVITY_MAGNETOMETER_4;
      break;
    case M_SCALE_8GS:
      mRes = SENSITIVITY_MAGNETOMETER_8;
      break;
    case M_SCALE_12GS:
      mRes = SENSITIVITY_MAGNETOMETER_12;
      break;
    case M_SCALE_16GS:
      mRes = SENSITIVITY_MAGNETOMETER_16;
      break;
  }
}

uint8_t lsm9ds1_whoami_ag() {
  return i2c_reg_read(settings.device.agAddress, WHO_AM_I_XG);
}

uint8_t lsm9ds1_whoami_m() {
  return i2c_reg_read(settings.device.mAddress, WHO_AM_I_M);
}

void initGyro() {
  uint8_t tempRegValue = 0;

  // CTRL_REG1_G (Default value: 0x00)
  // [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
  // ODR_G[2:0] - Output data rate selection
  // FS_G[1:0] - Gyroscope full-scale selection
  // BW_G[1:0] - Gyroscope bandwidth selection

  // To disable gyro, set sample rate bits to 0. We'll only set sample
  // rate if the gyro is enabled.
  if (settings.gyro.enabled)
  {
    tempRegValue = (settings.gyro.sampleRate & 0x07) << 5;
  }
  tempRegValue |= settings.gyro.scale << 3;
  tempRegValue |= (settings.gyro.bandwidth & 0x3);
  i2c_reg_write(settings.device.agAddress, CTRL_REG1_G, tempRegValue);

  // CTRL_REG2_G (Default value: 0x00)
  // [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
  // INT_SEL[1:0] - INT selection configuration
  // OUT_SEL[1:0] - Out selection configuration
  i2c_reg_write(settings.device.agAddress, CTRL_REG2_G, 0x00);

  // CTRL_REG3_G (Default value: 0x00)
  // [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
  // LP_mode - Low-power mode enable (0: disabled, 1: enabled)
  // HP_EN - HPF enable (0:disabled, 1: enabled)
  // HPCF_G[3:0] - HPF cutoff frequency
  tempRegValue = settings.gyro.lowPowerEnable ? (1<<7) : 0;
  if (settings.gyro.HPFEnable)
  {
    tempRegValue |= (1<<6) | (settings.gyro.HPFCutoff & 0x0F);
  }
  i2c_reg_write(settings.device.agAddress, CTRL_REG3_G, tempRegValue);

  // CTRL_REG4 (Default value: 0x38)
  // [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
  // Zen_G - Z-axis output enable (0:disable, 1:enable)
  // Yen_G - Y-axis output enable (0:disable, 1:enable)
  // Xen_G - X-axis output enable (0:disable, 1:enable)
  // LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
  // 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)
  tempRegValue = 0;
  if (settings.gyro.enableZ) tempRegValue |= (1<<5);
  if (settings.gyro.enableY) tempRegValue |= (1<<4);
  if (settings.gyro.enableX) tempRegValue |= (1<<3);
  if (settings.gyro.latchInterrupt) tempRegValue |= (1<<1);
  i2c_reg_write(settings.device.agAddress, CTRL_REG4, tempRegValue);

  // ORIENT_CFG_G (Default value: 0x00)
  // [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
  // SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
  // Orient [2:0] - Directional user orientation selection
  tempRegValue = 0;
  if (settings.gyro.flipX) tempRegValue |= (1<<5);
  if (settings.gyro.flipY) tempRegValue |= (1<<4);
  if (settings.gyro.flipZ) tempRegValue |= (1<<3);
  i2c_reg_write(settings.device.agAddress, ORIENT_CFG_G, tempRegValue);
}

void initAccel() {
  uint8_t tempRegValue = 0;

  //    CTRL_REG5_XL (0x1F) (Default value: 0x38)
  //    [DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
  //    DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
  //    00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
  //    Zen_XL - Z-axis output enabled
  //    Yen_XL - Y-axis output enabled
  //    Xen_XL - X-axis output enabled
  if (settings.accel.enableZ) tempRegValue |= (1<<5);
  if (settings.accel.enableY) tempRegValue |= (1<<4);
  if (settings.accel.enableX) tempRegValue |= (1<<3);

  i2c_reg_write(settings.device.agAddress, CTRL_REG5_XL, tempRegValue);

  // CTRL_REG6_XL (0x20) (Default value: 0x00)
  // [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
  // ODR_XL[2:0] - Output data rate & power mode selection
  // FS_XL[1:0] - Full-scale selection
  // BW_SCAL_ODR - Bandwidth selection
  // BW_XL[1:0] - Anti-aliasing filter bandwidth selection
  tempRegValue = 0;
  // To disable the accel, set the sampleRate bits to 0.
  if (settings.accel.enabled)
  {
    tempRegValue |= (settings.accel.sampleRate & 0x07) << 5;
  }
  tempRegValue |= settings.accel.scale << 3;
  if (settings.accel.bandwidth >= 0)
  {
    tempRegValue |= (1<<2); // Set BW_SCAL_ODR
    tempRegValue |= (settings.accel.bandwidth & 0x03);
  }
  i2c_reg_write(settings.device.agAddress, CTRL_REG6_XL, tempRegValue);

  // CTRL_REG7_XL (0x21) (Default value: 0x00)
  // [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
  // HR - High resolution mode (0: disable, 1: enable)
  // DCF[1:0] - Digital filter cutoff frequency
  // FDS - Filtered data selection
  // HPIS1 - HPF enabled for interrupt function
  tempRegValue = 0;
  if (settings.accel.highResEnable)
  {
    tempRegValue |= (1<<7); // Set HR bit
    tempRegValue |= (settings.accel.highResBandwidth & 0x3) << 5;
  }
  i2c_reg_write(settings.device.agAddress, CTRL_REG7_XL, tempRegValue);
}

void initMag() {
  uint8_t tempRegValue = 0;

  // CTRL_REG1_M (Default value: 0x10)
  // [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][0][ST]
  // TEMP_COMP - Temperature compensation
  // OM[1:0] - X & Y axes op mode selection
  // 00:low-power, 01:medium performance
  // 10: high performance, 11:ultra-high performance
  // DO[2:0] - Output data rate selection
  // ST - Self-test enable
  if (settings.mag.tempCompensationEnable) tempRegValue |= (1<<7);
  tempRegValue |= (settings.mag.XYPerformance & 0x3) << 5;
  tempRegValue |= (settings.mag.sampleRate & 0x7) << 2;
  i2c_reg_write(settings.device.mAddress, CTRL_REG1_M, tempRegValue);

  // CTRL_REG2_M (Default value 0x00)
  // [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
  // FS[1:0] - Full-scale configuration
  // REBOOT - Reboot memory content (0:normal, 1:reboot)
  // SOFT_RST - Reset config and user registers (0:default, 1:reset)
  tempRegValue = 0;
  tempRegValue |= settings.mag.scale;
  i2c_reg_write(settings.device.mAddress, CTRL_REG2_M, tempRegValue);

  // CTRL_REG3_M (Default value: 0x03)
  // [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
  // I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
  // LP - Low-power mode cofiguration (1:enable)
  // SIM - SPI mode selection (0:write-only, 1:read/write enable)
  // MD[1:0] - Operating mode
  //	00:continuous conversion, 01:single-conversion,
  //  10,11: Power-down
  tempRegValue = 0;
  if (settings.mag.lowPowerEnable) tempRegValue |= (1<<5);
  tempRegValue |= (settings.mag.operatingMode & 0x3);
  i2c_reg_write(settings.device.mAddress, CTRL_REG3_M, tempRegValue); // Continuous conversion mode

  // CTRL_REG4_M (Default value: 0x00)
  // [0][0][0][0][OMZ1][OMZ0][BLE][0]
  // OMZ[1:0] - Z-axis operative mode selection
  //	00:low-power mode, 01:medium performance
  //	10:high performance, 10:ultra-high performance
  // BLE - Big/little endian data
  tempRegValue = 0;
  tempRegValue = (settings.mag.ZPerformance & 0x3) << 2;
  i2c_reg_write(settings.device.mAddress, CTRL_REG4_M, tempRegValue);

  // CTRL_REG5_M (Default value: 0x00)
  // [0][BDU][0][0][0][0][0][0]
  // BDU - Block data update for magnetic data
  //	0:continuous, 1:not updated until MSB/LSB are read
  tempRegValue = 0;
  i2c_reg_write(settings.device.mAddress, CTRL_REG5_M, tempRegValue);
}

// initialization and configuration
ret_code_t lsm9ds1_init(uint8_t agAddress, uint8_t mAddress, const nrf_twi_mngr_t* i2c) {
  // Set device settings, they are used in many other places
  settings.device.commInterface = IMU_MODE_I2C;
  settings.device.agAddress = agAddress;
  settings.device.mAddress = mAddress;
  settings.device.i2c = i2c;

  settings.gyro.enabled = true;
  settings.gyro.enableX = true;
  settings.gyro.enableY = true;
  settings.gyro.enableZ = true;
  // gyro scale can be 245, 500, or 2000
  settings.gyro.scale = G_SCALE_245DPS;
  // gyro sample rate: value between 1-6
  // 1 = 14.9    4 = 238
  // 2 = 59.5    5 = 476
  // 3 = 119     6 = 952
  settings.gyro.sampleRate = G_ODR_952;
  // gyro cutoff frequency: value between 0-3
  // Actual value of cutoff frequency depends
  // on sample rate.
  settings.gyro.bandwidth = 0;
  settings.gyro.lowPowerEnable = false;
  settings.gyro.HPFEnable = false;
  // Gyro HPF cutoff frequency: value between 0-9
  // Actual value depends on sample rate. Only applies
  // if gyroHPFEnable is true.
  settings.gyro.HPFCutoff = 0;
  settings.gyro.flipX = false;
  settings.gyro.flipY = false;
  settings.gyro.flipZ = false;
  settings.gyro.orientation = 0;
  settings.gyro.latchInterrupt = true;

  settings.accel.enabled = true;
  settings.accel.enableX = true;
  settings.accel.enableY = true;
  settings.accel.enableZ = true;
  // accel scale can be 2, 4, 8, or 16G
  settings.accel.scale = A_SCALE_2G;
  // accel sample rate can be 1-6
  // 1 = 10 Hz    4 = 238 Hz
  // 2 = 50 Hz    5 = 476 Hz
  // 3 = 119 Hz   6 = 952 Hz
  settings.accel.sampleRate = XL_ODR_952;
  // Accel cutoff freqeuncy can be any value between -1 - 3.
  // -1 = bandwidth determined by sample rate
  // 0 = 408 Hz   2 = 105 Hz
  // 1 = 211 Hz   3 = 50 Hz
  settings.accel.bandwidth = -1;
  settings.accel.highResEnable = false;
  // accelHighResBandwidth can be any value between 0-3
  // LP cutoff is set to a factor of sample rate
  // 0 = ODR/50    2 = ODR/9
  // 1 = ODR/100   3 = ODR/400
  settings.accel.highResBandwidth = 0;

  settings.mag.enabled = true;
  // mag scale can be 4, 8, 12, or 16
  settings.mag.scale = M_SCALE_4GS;
  // mag data rate can be 0-7
  // 0 = 0.625 Hz  4 = 10 Hz
  // 1 = 1.25 Hz   5 = 20 Hz
  // 2 = 2.5 Hz    6 = 40 Hz
  // 3 = 5 Hz      7 = 80 Hz
  settings.mag.sampleRate = M_ODR_80;
  settings.mag.tempCompensationEnable = false;
  // magPerformance can be any value between 0-3
  // 0 = Low power mode      2 = high performance
  // 1 = medium performance  3 = ultra-high performance
  settings.mag.XYPerformance = 3;
  settings.mag.ZPerformance = 3;
  settings.mag.lowPowerEnable = false;
  // magOperatingMode can be 0-2
  // 0 = continuous conversion
  // 1 = single-conversion
  // 2 = power down
  settings.mag.operatingMode = 0;

  settings.temp.enabled = true;
  for (int i=0; i<3; i++)
  {
    gBias[i] = 0;
    aBias[i] = 0;
    mBias[i] = 0;
    gBiasRaw[i] = 0;
    aBiasRaw[i] = 0;
    mBiasRaw[i] = 0;
  }
  autocalc = false;

  // Using the ODR of each sensor, We can calculate the resolution
  // That's what these functions are for. One for each sensor
  calcgRes(); // Calculate DPS / ADC tick, stored in gRes variable
  calcmRes(); // Calculate Gs / ADC tick, stored in mRes variable
  calcaRes(); // Calculate g / ADC tick, stored in aRes variable

  // To verify communication, we can read from the WHO_AM_I register of
  // each device. Store those in a variable so we can return them.
  uint8_t xgTest = lsm9ds1_whoami_ag();         // Read the accel/gyro WHO_AM_I
  uint8_t mTest  = lsm9ds1_whoami_m();          // Read the mag WHO_AM_I
  uint16_t whoAmICombined = (xgTest << 8) | mTest;

  if (whoAmICombined != ((WHO_AM_I_AG_RSP << 8) | WHO_AM_I_M_RSP))
    return -1;

  // Gyro initialization stuff:
  initGyro();    // This will "turn on" the gyro. Setting up interrupts, etc.

  // Accelerometer initialization stuff:
  initAccel(); // "Turn on" all axes of the accel. Set up interrupts, etc.

  // Magnetometer initialization stuff:
  initMag(); // "Turn on" all axes of the mag. Set up interrupts, etc.

  return NRF_SUCCESS;
}

lsm9ds1_measurement lsm9ds1_read_accelerometer() {
  uint8_t temp[6];
  lsm9ds1_measurement meas = {0};

  i2c_read_bytes(settings.device.agAddress, OUT_X_L_XL, temp, 6);
  for (size_t i = 0; i < 6; i++) {
    printf("%x ", temp[i]);
  }
  printf("\n");
  ax = (temp[1] << 8) | temp[0];
  ay = (temp[3] << 8) | temp[2];
  az = (temp[5] << 8) | temp[4];
  if (autocalc) {
    ax -= aBiasRaw[X_AXIS];
    ay -= aBiasRaw[Y_AXIS];
    az -= aBiasRaw[Z_AXIS];
  }

  meas.x_axis = ax * aRes;
  meas.y_axis = ay * aRes;
  meas.z_axis = az * aRes;
  return meas;
}

