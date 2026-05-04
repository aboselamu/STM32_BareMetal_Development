#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f4xx.h" // Replace with your specific MCU header

/* Event Definitions for Callbacks */
#define USART_EVENT_TX_CMPLT  0
#define USART_EVENT_RX_CMPLT  1
#define USART_EVENT_ERROR     3

/* State Definitions */
typedef enum {
    USART_READY,
    USART_BUSY_TX,
    USART_BUSY_RX
} USART_State_t;

/* Configuration Structure */
typedef struct {
    uint32_t BaudRate;
    uint32_t StopBits;   // 0: 1-bit, 2: 2-bits
    uint32_t WordLength; // 0: 8-bits
} USART_Config_t;

/* Handle Structure (The "Car Folder") */
typedef struct {
    USART_TypeDef  *pUSARTx;      // Base address of USART hardware
    USART_Config_t  USART_Config; 
    uint8_t        *pTxBuffer;
    uint8_t        *pRxBuffer;
    uint32_t        TxLen;
    uint32_t        RxLen;
    USART_State_t   TxState;
    USART_State_t   RxState;
    void (*pCallback)(uint8_t event); // The Callback "Phone Number"
} USART_Handle_t;

/* Function Prototypes */
void USART_Init(USART_Handle_t *pHandle);
void USART_SendData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len);
void USART_ReceiveData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len);
void USART_IRQHandling(USART_Handle_t *pHandle); // The "Generic Mechanic"

#endif