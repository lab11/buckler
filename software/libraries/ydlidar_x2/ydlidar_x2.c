#include "ydlidar_x2.h"

static YdLidarData_t data;
static lidar_struct data_struct;

float to_distance(uint8_t Si) {
    return float(SI / 4);
}

float start_angle(uint8_t FSA) {
}

float end_angle(uint8_t LSA) {
    
}

void parse_lidar(uint8_t lidar_data[522]) {
    data_struct.PH_LSB = lidar_data[0];
    data_struct.PH_MSB = lidar_data[1];
    data_struct.CT = lidar_data[2];
    data_struct.LS = lidar_data[3];
    data_struct.FSA_LSB = lidar_data[4];
    data_struct.FSA_MSB = lidar_data[5];
    data_struct.LSA_LSB = lidar_data[6];
    data_struct.LSA_MSB = lidar_data[7];
    data_struct.CS_LSB = lidar_data[8];
    data_struct.CS_MSB = lidar_data[9];
    for (uint16_t i = 0; i < LS; i++) {
        data_struct.Si_LSB[] = lidar_data[10 + i];
        data_struct.Si_MSB[] = lidar_data[11 + i];
    }
}

void update_data() {

}

YdLidarData_t* get_lidar_data() {
    u_int8_t lidar_data[522]; //= get_data from ic2
    parse_lidar(lidar_data);
    return &data;
}