#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include "stm32f4xx.h" 

/* @ref USART_Event_Definitions */
#define USART_EVENT_TX_CMPLT  0
#define USART_EVENT_RX_CMPLT  1
#define USART_EVENT_IDLE      2
#define USART_EVENT_ERROR     3

/* @ref USART_States */
typedef enum {
    USART_READY,
    USART_BUSY_TX,
    USART_BUSY_RX
} USART_State_t;

/* Configuration Structure */
typedef struct {
    uint32_t BaudRate;
    uint32_t StopBits;   // Use standard values (e.g., 0 for 1 stop bit)
    uint32_t WordLength; // 0 for 8 bits, 1 for 9 bits
    uint32_t Mode;       // RX only, TX only, or both
} USART_Config_t;

/* Handle Structure (The Opaque Object) */
typedef struct {
    USART_TypeDef  *pUSARTx;      
    USART_Config_t  USART_Config; 
    uint8_t        *pTxBuffer;
    uint8_t        *pRxBuffer;
    uint32_t        TxLen;
    uint32_t        RxLen;
    volatile uint8_t TxState;     // volatile is mandatory for ISR variables
    volatile uint8_t RxState;
    void (*pApplicationCallback)(uint8_t event); 
} USART_Handle_t;

/* API Prototypes */
void USART_Init(USART_Handle_t *pHandle);
void USART_SendData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len);
void USART_ReceiveData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len);
void USART_IRQHandling(USART_Handle_t *pHandle);

/* MSP Hook - To be implemented by the user in the board file */
void USART_MspInit(USART_Handle_t *pHandle);

#endif
