#include "spi_driver.h"
#include <stdio.h>
#include "pwm_driver.h"

#include "board.h"        // The "Plumbing" contract
#include "usart_driver.h"  // The "Logic" contract


SPI_Handle_t hspi1;
uint8_t master_tx[] = "Hello ESP32!";
uint8_t master_rx[13];

void My_SPI_CpltCallback(SPI_Handle_t *hspi) {
    if (hspi->Instance == SPI1) {
        printf("Transfer Complete!\n");


/* 1. GLOBAL HANDLES & BUFFERS */
USART_Handle_t myUsart2Handle;
uint8_t rxBuffer[10]; // Small buffer for incoming data

/* 2. THE APPLICATION CALLBACK */
/**
 * @brief This is your logic "Hook."
 * When the driver finishes a hardware task, it calls this function.
 */
void My_USART_Callback(uint8_t event) {
    if (event == USART_EVENT_RX_CMPLT) {
        // APPLICATION LOGIC: Echo the data back to the PC
        // We use the driver API to send, staying decoupled from registers.
        USART_SendData_IT(&myUsart2Handle, rxBuffer, 10);
        
        // Re-enable reception to wait for the next 10 bytes
        USART_ReceiveData_IT(&myUsart2Handle, rxBuffer, 10);
    }

    if (event == USART_EVENT_TX_CMPLT) {
        // Logic for when transmission finishes (e.g., toggle an LED)
    }

    if (event == USART_EVENT_ERROR) {
        // Handle communication errors (Overrun, Noise, etc.)
    }
}

/* 3. MAIN APPLICATION */
int main(void) {

    /* --- Driver Configuration --- */
    hspi1.Instance = SPI1;
    hspi1.Config.mode = 1;         
    hspi1.Config.bus_config = 1;   
    hspi1.Config.prescaler = 4;    
    hspi1.Config.data_size = 8;    
    hspi1.TxRxCpltCallback = My_SPI_CpltCallback;

    /* Init triggers board.c to configure clocks and pins */
    SPI_Init(&hspi1);

    /* Start transfer in the background */
    if (SPI_Transfer_IT(&hspi1, master_tx, master_rx, 12)) {
        printf("SPI Transfer started in background...\n");
    }

    while (1) {
        // CPU is free for other tasks

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

    /* --- STEP A: CONFIGURATION --- */
    // We define "What" we want, but not "How" the hardware is wired.
    myUsart2Handle.pUSARTx = USART2;
    myUsart2Handle.USART_Config.BaudRate = 0x0683; // 9600 @ 16MHz
    myUsart2Handle.USART_Config.StopBits = 0;      // 1 Stop Bit
    myUsart2Handle.USART_Config.WordLength = 0;    // 8-bit Data
    myUsart2Handle.USART_Config.Mode = 3;           // RX and TX enabled
    
    // Register the callback "phone number"
    myUsart2Handle.pApplicationCallback = My_USART_Callback;

    /* --- STEP B: INITIALIZATION --- */
    /** 
     * This call triggers the Driver -> Board (MSP) handshake.
     * The driver calls USART_MspInit() inside board.c to setup pins.
     */
    USART_Init(&myUsart2Handle);

    /* --- STEP C: START THE ENGINE --- */
    // Non-blocking start: The CPU stays free for other tasks
    USART_ReceiveData_IT(&myUsart2Handle, rxBuffer, 10);

    while(1) {
        // The main loop is empty because the system is Event-Driven.
        // You could put low-priority background tasks here.
    }
}
