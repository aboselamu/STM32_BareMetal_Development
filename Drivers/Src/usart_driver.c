#include "usart_driver.h"

/* Weak implementation allows the user to override this without compiler errors */
__weak void USART_MspInit(USART_Handle_t *pHandle) {
    (void)pHandle; // Default empty implementation
}

void USART_Init(USART_Handle_t *pHandle) {
    // 1. Board-Specific Setup (Clocks, Pins, NVIC)
    USART_MspInit(pHandle);

    // 2. Configure Baud Rate
    pHandle->pUSARTx->BRR = pHandle->USART_Config.BaudRate;

    // 3. Configure CR1 (Word Length, TX/RX Enable, Peripheral Enable)
    uint32_t tempReg = 0;
    tempReg |= (pHandle->USART_Config.Mode << USART_CR1_RE_Pos); // Enables RE/TE
    if(pHandle->USART_Config.WordLength) tempReg |= USART_CR1_M;
    
    // Always enable RXNE interrupt and the Peripheral itself
    tempReg |= USART_CR1_RXNEIE | USART_CR1_UE; 
    pHandle->pUSARTx->CR1 = tempReg;

    // 4. Configure CR2 (Stop Bits)
    pHandle->pUSARTx->CR2 &= ~USART_CR2_STOP; 
    pHandle->pUSARTx->CR2 |= (pHandle->USART_Config.StopBits << USART_CR2_STOP_Pos);

    pHandle->TxState = USART_READY;
    pHandle->RxState = USART_READY;
}

void USART_SendData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len) {
    if(pHandle->TxState != USART_READY) return;

    pHandle->pTxBuffer = pBuffer;
    pHandle->TxLen = Len;
    pHandle->TxState = USART_BUSY_TX;

    // Trigger the process by enabling Transmit Data Register Empty interrupt
    pHandle->pUSARTx->CR1 |= USART_CR1_TXEIE;
}

void USART_IRQHandling(USART_Handle_t *pHandle) {
    uint32_t sr = pHandle->pUSARTx->SR;
    uint32_t cr1 = pHandle->pUSARTx->CR1;

    // Handle RXNE (Data Received)
    if((sr & USART_SR_RXNE) && (cr1 & USART_CR1_RXNEIE)) {
        if(pHandle->RxState == USART_BUSY_RX) {
            *pHandle->pRxBuffer++ = (uint8_t)pHandle->pUSARTx->DR;
            if(--pHandle->RxLen == 0) {
                pHandle->RxState = USART_READY;
                if(pHandle->pApplicationCallback) pHandle->pApplicationCallback(USART_EVENT_RX_CMPLT);
            }
        } else {
            (void)pHandle->pUSARTx->DR; // Clear flag if no RX was requested
        }
    }

    // Handle TXE (Ready to send next byte)
    if((sr & USART_SR_TXE) && (cr1 & USART_CR1_TXEIE)) {
        if(pHandle->TxState == USART_BUSY_TX) {
            pHandle->pUSARTx->DR = *pHandle->pTxBuffer++;
            if(--pHandle->TxLen == 0) {
                pHandle->pUSARTx->CR1 &= ~USART_CR1_TXEIE; // Disable TXE
                pHandle->pUSARTx->CR1 |= USART_CR1_TCIE;   // Enable Transmission Complete
            }
        }
    }

    // Handle TC (Transmission Complete - physical line is clear)[cite: 1]
    if((sr & USART_SR_TC) && (cr1 & USART_CR1_TCIE)) {
        pHandle->pUSARTx->CR1 &= ~USART_CR1_TCIE;
        pHandle->TxState = USART_READY;
        if(pHandle->pApplicationCallback) pHandle->pApplicationCallback(USART_EVENT_TX_CMPLT);
    }
}
