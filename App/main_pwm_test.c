#include "board.h"
#include "pwm_driver.h"

int main(void)
{
    // 1. Setup Pins
    GPIO_Handle_t pwm1_pin = {GPIOA, 6, 2}; // TIM3_CH1
    Board_GPIO_AF_Init(&pwm1_pin);

    // 2. Setup PWM Handle
    PWM_HandleTypeDef hpwm1;
    hpwm1.Instance = TIM3;
    hpwm1.channel  = 1;
    hpwm1.Config.prescaler = 71;
    hpwm1.Config.period    = 999;
    hpwm1.Config.polarity  = 0;

    // 3. Initialize and check status
    if (PWM_Init(&hpwm1) != PWM_STATUS_OK) 
    {
        // Error handling: Stay here if hardware init fails
        while(1); 
    }

    while (1)
    {
        // Smoothly update duty cycle
        PWM_SetDuty(&hpwm1, 500);
    }
}