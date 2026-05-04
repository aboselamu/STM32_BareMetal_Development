#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>

/* Trigger Macros */
#define EXTI_TRIGGER_RISING  0
#define EXTI_TRIGGER_FALLING 1
#define EXTI_TRIGGER_BOTH    2[cite: 10]

typedef struct {
    uint8_t line;           // 0..15[cite: 11]
    uint8_t trigger;        // Rising, Falling, or Both[cite: 11]
    GPIO_TypeDef *pPort;    // GPIOA, GPIOB, etc.[cite: 11]
    void (*pApplicationCallback)(uint8_t line); // The Hook[cite: 10]
} EXTI_Handle_t;

/* API */
void EXTI_Init(EXTI_Handle_t *pHandle);
void EXTI_IRQHandling(uint8_t line); // Called by stm32_it.c

/* MSP Hook - Implemented in board.c */
void EXTI_MspInit(EXTI_Handle_t *pHandle);

#endif