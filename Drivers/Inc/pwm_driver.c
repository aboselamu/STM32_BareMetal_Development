#include "pwm_driver.h"

// -------- Clock enable --------
static void PWM_EnableClock(TIM_TypeDef *tim)
{
    if (tim == TIM2)      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    else if (tim == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    else if (tim == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

// -------- INIT --------
PWM_Status_t PWM_Init(PWM_HandleTypeDef *hpwm)
{
    // 1. Safety Check
    if (hpwm == NULL || hpwm->Instance == NULL) return PWM_STATUS_ERROR;
    if (hpwm->channel < 1 || hpwm->channel > 4) return PWM_STATUS_ERROR;

    // 2. Enable timer clock
    PWM_EnableClock(hpwm->Instance);

    // 3. Base hardware config
    hpwm->Instance->PSC = hpwm->Config.prescaler;
    hpwm->Instance->ARR = hpwm->Config.period;
    hpwm->Instance->EGR |= TIM_EGR_UG;      
    hpwm->Instance->CR1 |= TIM_CR1_ARPE;    

    // 4. Channel config logic
    uint8_t ch = hpwm->channel - 1;
    uint8_t shift = (ch & 1) ? 12 : 4; 
    volatile uint32_t *ccmr = (ch < 2) ? &hpwm->Instance->CCMR1 : &hpwm->Instance->CCMR2;

    // Clear and Set PWM Mode 1
    *ccmr &= ~(0x7U << shift);
    *ccmr |=  (TIM_CCMR_OCMODE_PWM1 << shift); 
    
    // Enable Preload (allows ARR/CCR updates to sync with timer overflow)
    *ccmr |=  (TIM_CCMR_OC_PRELOAD_EN << shift);

    // Channel Enable and Polarity
    uint8_t ccer_shift = ch * 4;
    hpwm->Instance->CCER |= (1U << (ccer_shift + TIM_CCER_CCxE_BIT)); 

    if (hpwm->Config.polarity)
        hpwm->Instance->CCER |= (1U << (ccer_shift + TIM_CCER_CCxP_BIT));
    else
        hpwm->Instance->CCER &= ~(1U << (ccer_shift + TIM_CCER_CCxP_BIT));

    // 5. Start timer
    hpwm->Instance->CR1 |= TIM_CR1_CEN;

    return PWM_STATUS_OK;
}

// -------- RUNTIME --------
PWM_Status_t PWM_SetDuty(PWM_HandleTypeDef *hpwm, uint32_t duty)
{
    if (hpwm == NULL || hpwm->Instance == NULL) return PWM_STATUS_ERROR;

    // Safety: Clamp duty to period
    if (duty > hpwm->Config.period) duty = hpwm->Config.period;

    volatile uint32_t *ccr = &hpwm->Instance->CCR1 + (hpwm->channel - 1);
    *ccr = duty;

    return PWM_STATUS_OK;
}

void PWM_Start(PWM_HandleTypeDef *hpwm) {
    if (hpwm != NULL) hpwm->Instance->CR1 |= TIM_CR1_CEN;
}

void PWM_Stop(PWM_HandleTypeDef *hpwm) {
    if (hpwm != NULL) hpwm->Instance->CR1 &= ~TIM_CR1_CEN;
}