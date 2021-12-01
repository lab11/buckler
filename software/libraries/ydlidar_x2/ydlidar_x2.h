#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_timer.h"
#include "nrf_twi_mngr.h"

typedef struct {
    uint8_t PH[2];
    uint8_t CT;
    uint8_t LS;
    uint8_t FSA[2];
    uint8_t LSA[2]:
    uint8_t CS[2];
    uint8_t Si[256][2];
} YdLidarData_t;

