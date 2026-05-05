#include "board.h"
#include "spi_driver.h"
#include <stdio.h>

SPI_Handle_t hspi1;
uint8_t master_tx[] = "Hello ESP32!";
uint8_t master_rx[13];

void My_SPI_CpltCallback(SPI_Handle_t *hspi) {
    if (hspi->Instance == SPI1) {
        printf("Transfer Complete!\n");[cite: 13]
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
        // Tell driver to wait for the next byte[cite: 1, 9]
        USART_ReceiveData_IT(&dimUsartHandle, &rx_char, 1);
    }
}

int main(void) {

    /* --- Driver Configuration --- */
    hspi1.Instance = SPI1;
    hspi1.Config.mode = 1;         
    hspi1.Config.bus_config = 1;   
    hspi1.Config.prescaler = 4;    
    hspi1.Config.data_size = 8;    
    hspi1.TxRxCpltCallback = My_SPI_CpltCallback;[cite: 13]

    /* Init triggers board.c to configure clocks and pins */
    SPI_Init(&hspi1);[cite: 13]

    /* Start transfer in the background */
    if (SPI_Transfer_IT(&hspi1, master_tx, master_rx, 12)) {
        printf("SPI Transfer started in background...\n");[cite: 13]
    }

    while (1) {
        // CPU is free for other tasks

    // PWM Configuration[cite: 5, 8]
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