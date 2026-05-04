#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include <stdint.h>

typedef struct {
    uint8_t line;           // 0..15
    uint8_t trigger;        // 0=rising, 1=falling, 2=both
    uint32_t gpio_port;     // base address, e.g. GPIOA_BASE
    uint8_t gpio_pin;       // 0..15 (should match line for lines 0..15)
} exti_config_t;

void exti_init(const exti_config_t *cfg);
void exti_set_callback(void (*callback)(uint8_t line));

#endif