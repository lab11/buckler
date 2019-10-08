#include "gpio.h"

GPIO_t* GPIO_ptr = (GPIO_t*) GPIO_BASE_ADDR;

// Inputs:
//  gpio_num - gpio number 0-31
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  if (dir == INPUT) {
    GPIO_ptr->PIN_CNF[gpio_num] &= ~0x3;
  } else {
    GPIO_ptr->PIN_CNF[gpio_num] |= 0x3;
  }
}

// Set gpio_num high
// Inputs:
//  gpio_num - gpio number 0-31
void gpio_set(uint8_t gpio_num) {
  GPIO_ptr->OUT |= 0x1 << gpio_num;
}

// Set gpio_num low
// Inputs:
//  gpio_num - gpio number 0-31
void gpio_clear(uint8_t gpio_num) {
  GPIO_ptr->OUT &= ~(0x1 << gpio_num);
}

// Inputs:
//  gpio_num - gpio number 0-31
bool gpio_read(uint8_t gpio_num) {
    // should return pin state
    return (GPIO_ptr->IN >> gpio_num) & 0x1;
}
