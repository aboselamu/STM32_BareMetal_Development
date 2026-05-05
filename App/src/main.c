#include "board.h"        // The "Plumbing" contract
#include "usart_driver.h"  // The "Logic" contract

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
        USART_SendData_IT(&myUsart2Handle, rxBuffer, 10);[cite: 1, 3]
        
        // Re-enable reception to wait for the next 10 bytes
        USART_ReceiveData_IT(&myUsart2Handle, rxBuffer, 10);[cite: 3]
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
    /* --- STEP A: CONFIGURATION --- */
    // We define "What" we want, but not "How" the hardware is wired.
    myUsart2Handle.pUSARTx = USART2;
    myUsart2Handle.USART_Config.BaudRate = 0x0683; // 9600 @ 16MHz
    myUsart2Handle.USART_Config.StopBits = 0;      // 1 Stop Bit
    myUsart2Handle.USART_Config.WordLength = 0;    // 8-bit Data[cite: 4]
    myUsart2Handle.USART_Config.Mode = 3;           // RX and TX enabled
    
    // Register the callback "phone number"[cite: 4]
    myUsart2Handle.pApplicationCallback = My_USART_Callback;

    /* --- STEP B: INITIALIZATION --- */
    /** 
     * This call triggers the Driver -> Board (MSP) handshake.
     * The driver calls USART_MspInit() inside board.c to setup pins.
     */
    USART_Init(&myUsart2Handle);[cite: 1, 3]

    /* --- STEP C: START THE ENGINE --- */
    // Non-blocking start: The CPU stays free for other tasks
    USART_ReceiveData_IT(&myUsart2Handle, rxBuffer, 10);[cite: 3]

    while(1) {
        // The main loop is empty because the system is Event-Driven.
        // You could put low-priority background tasks here.
    }
}