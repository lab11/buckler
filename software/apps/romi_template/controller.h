typedef enum {
  OFF,
  DRIVING,
} robot_state_t;

static float measure_distance(uint16_t current_encoder, uint16_t previous_encoder);

robot_state_t controller(robot_state_t state);