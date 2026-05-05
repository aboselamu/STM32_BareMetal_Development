#include "board.h"
#include "usart_driver.h"
#include "pwm_driver.h"

// 1. Global Handles (Live for the life of the app)
USART_Handle_t dimUsartHandle;
PWM_HandleTypeDef dimPwmHandle;
uint8_t rx_char; 

// 2. The "Brain" (The Callback)
void App_USART_Callback(uint8_t event) {
    if (event == USART_EVENT_RX_CMPLT) {
        if (rx_char >= '0' && rx_char <= '4') {
            uint32_t duty = ((rx_char - '0') * dimPwmHandle.Config.period) / 4;
            PWM_SetDuty(&dimPwmHandle, duty); // Generic API call
        }
        // Tell driver to wait for the next byte
        USART_ReceiveData_IT(&dimUsartHandle, &rx_char, 1);
    }
}

int main(void) {
    // PWM Configuration
    dimPwmHandle.Instance = TIM2;
    dimPwmHandle.channel  = 1;
    dimPwmHandle.Config.prescaler = 15; 
    dimPwmHandle.Config.period    = 999;
    PWM_Init(&dimPwmHandle);

    // USART Configuration
    dimUsartHandle.pUSARTx = USART2;
    dimUsartHandle.USART_Config.BaudRate = 0x0683; // 9600
    dimUsartHandle.pCallback = App_USART_Callback;
    USART_Init(&dimUsartHandle);

    // Start listening
    USART_ReceiveData_IT(&dimUsartHandle, &rx_char, 1);

    while (1) {
        // App is purely interrupt driven; main loop is free!
    }
}
