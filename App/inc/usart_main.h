#ifndef BOARD_H
#define BOARD_H

#include "stm32f4xx.h"
#include <stdint.h>

typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
    uint8_t af;
} GPIO_Handle_t;

void Board_GPIO_AF_Init(GPIO_Handle_t *gpio);

#endif