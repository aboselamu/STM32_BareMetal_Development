#include "pwm_driver.h"

__weak void PWM_MspInit(PWM_HandleTypeDef *hpwm) {
    (void)hpwm; // Default empty implementation
}

PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm) {
    if (hpwm == NULL || hpwm->Instance == NULL) return PWM_STATUS_ERROR;

    // 1. Trigger Board-level setup (Clock and Pins)
    PWM_MspInit(hpwm);

    // 2. Configure Time Base
    hpwm->Instance->PSC = hpwm->Config.prescaler;
    hpwm->Instance->ARR = hpwm->Config.period;
    hpwm->Instance->EGR |= TIM_EGR_UG;      // Generate update to load PSC
    hpwm->Instance->CR1 |= TIM_CR1_ARPE;    // Auto-reload preload enable[cite: 8, 9]

    // 3. Configure Channel Output Mode (PWM Mode 1)
    uint8_t ch = hpwm->channel - 1;
    uint8_t shift = (ch & 1) ? 12 : 4; 
    volatile uint32_t *ccmr = (ch < 2) ? &hpwm->Instance->CCMR1 : &hpwm->Instance->CCMR2;[cite: 8]

    *ccmr &= ~(0x7U << shift); // Clear bits
    *ccmr |= (0x6U << shift);  // PWM Mode 1[cite: 9]
    *ccmr |= (1U << (shift - 1)); // OCxPE: Output Compare Preload Enable

    // 4. Enable Channel and Set Polarity[cite: 8]
    uint8_t ccer_shift = ch * 4;
    hpwm->Instance->CCER |= (1U << ccer_shift); // CCxE: Signal Output Enable

    if (hpwm->Config.polarity)
        hpwm->Instance->CCER |= (1U << (ccer_shift + 1)); // CCxP: Active Low
    else
        hpwm->Instance->CCER &= ~(1U << (ccer_shift + 1)); // CCxP: Active High

    // 5. Start Counter
    hpwm->Instance->CR1 |= TIM_CR1_CEN;[cite: 8, 9]

    return PWM_STATUS_OK;
}

PWM_Status_t PWM_SetDuty(PWM_HandleTypeDef *hpwm, uint32_t duty) {
    if (hpwm == NULL || hpwm->Instance == NULL) return PWM_STATUS_ERROR;

    // Clamp duty to period to avoid overflow
    if (duty > hpwm->Config.period) duty = hpwm->Config.period;

    // Direct register access via offset calculation
    volatile uint32_t *ccr = &hpwm->Instance->CCR1 + (hpwm->channel - 1);[cite: 8]
    *ccr = duty;

    return PWM_STATUS_OK;
}