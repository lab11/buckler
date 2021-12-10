#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_drv_timer.h"
#include "nrf_twi_mngr.h"

#define MAX_SAMPLES 256
#define RESOLUTION 429
typedef struct {
    float theta[RESOLUTION];
    float distance[RESOLUTION];
} YdLidarData_t;

typedef struct {
    uint8_t PH_LSB;
    uint8_t PH_MSB;
    uint8_t CT;
    uint8_t LS;
    uint8_t FSA_LSB;
    uint8_t FSA_MSB;
    uint8_t LSA_LSB;
    uint8_t LSA_MSB;
    uint8_t CS_LSB;
    uint8_t CS_MSB;
    uint8_t Si_LSB[MAX_SAMPLES];
    uint8_t Si_MSB[MAX_SAMPLES];
} lidar_struct;

float to_distance(float SI);



