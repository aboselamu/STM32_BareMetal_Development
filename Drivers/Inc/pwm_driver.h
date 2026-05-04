#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <stddef.h>
// -------- Register Bit Definitions --------

// CCMR: PWM Mode Definitions
#define TIM_CCMR_OCMODE_PWM1    (0x6U)
#define TIM_CCMR_OCMODE_PWM2    (0x7U)

// CCMR: Output Compare Preload Enable
#define TIM_CCMR_OC_PRELOAD_EN  (1U << 3)

// CCER: Capture/Compare Output Enable & Polarity
#define TIM_CCER_CCxE_BIT       (0U) // Offset for Enable bit
#define TIM_CCER_CCxP_BIT       (1U) // Offset for Polarity bit

// CR1: Control Register 1
#define TIM_CR1_ARPE_EN         (1U << 7) // Auto-reload preload enable
#define TIM_CR1_CEN_BIT         (1U << 0) // Counter enable

// The "Hole" is 3 bits wide (0x7)
#define TIM_CCMR_OCMODE_MASK    (0x7U)

// The "Values" that fit in the hole
#define TIM_CCMR_OCMODE_PWM1    (0x6U)
#define TIM_CCMR_OCMODE_PWM2    (0x7U)

// -------- STATUS TYPES --------
typedef enum {
    PWM_STATUS_OK      = 0x00,
    PWM_STATUS_ERROR   = 0x01,
    PWM_STATUS_BUSY    = 0x02
} PWM_Status_t;

// -------- CONFIG (behavior) --------
typedef struct {
    uint32_t prescaler;
    uint32_t period;
    uint8_t  polarity;
} PWM_Config_t;

// -------- HANDLE (resource + config) --------
typedef struct {
    TIM_TypeDef *Instance;   // Hardware register base (e.g., TIM3)
    uint8_t channel;         // CH1–CH4
    PWM_Config_t Config;     // Software configuration
} PWM_HandleTypeDef;

// -------- API --------
PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm);
PWM_Status_t PWM_SetDuty(PWM_HandleTypeDef *hpwm, uint32_t duty);
void PWM_Start(PWM_HandleTypeDef *hpwm);
void PWM_Stop(PWM_HandleTypeDef *hpwm);

#endif