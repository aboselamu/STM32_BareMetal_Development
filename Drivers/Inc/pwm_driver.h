#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>

/* -------- STATUS TYPES -------- */
typedef enum {
    PWM_STATUS_OK      = 0x00,
    PWM_STATUS_ERROR   = 0x01
} PWM_Status_t;

/* -------- CONFIG (behavior) -------- */
typedef struct {
    uint32_t prescaler;
    uint32_t period;
    uint8_t  polarity;  // 0: Active High, 1: Active Low
} PWM_Config_t;

/* -------- HANDLE (resource + config) -------- */
typedef struct {
    TIM_TypeDef *Instance;   // Hardware register base (e.g., TIM3)[cite: 9]
    uint8_t channel;         // CH1–CH4[cite: 9]
    PWM_Config_t Config;     
} PWM_HandleTypeDef;

/* -------- API -------- */
PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_SetDuty(PWM_HandleTypeDef *hpwm, uint32_t duty);

/* MSP Hook - To be implemented in board.c */
void PWM_MspInit(PWM_HandleTypeDef *hpwm);

#endif