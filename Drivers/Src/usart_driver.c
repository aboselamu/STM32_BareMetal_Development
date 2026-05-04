#include "usart_driver.h"

void USART_Init(USART_Handle_t *pHandle) {
    pHandle->TxState = USART_READY;
    pHandle->RxState = USART_READY;

    // 1. Set Baud Rate
    pHandle->pUSARTx->BRR = pHandle->USART_Config.BaudRate;
    
    // 2. Configure Stop Bits (CR2)
    pHandle->pUSARTx->CR2 &= ~(3 << 12);
    pHandle->pUSARTx->CR2 |= (pHandle->USART_Config.StopBits << 12);

    // 3. Enable Transmitter, Receiver, and RX-Not-Empty Interrupt
    pHandle->pUSARTx->CR1 |= (1 << 2) | (1 << 3) | (1 << 5);

    // 4. Global NVIC Enable (The Front Gate)
    if(pHandle->pUSARTx == USART1) NVIC_EnableIRQ(USART1_IRQn);
    else if(pHandle->pUSARTx == USART2) NVIC_EnableIRQ(USART2_IRQn);

    // 5. Enable the USART Peripheral (UE)
    pHandle->pUSARTx->CR1 |= (1 << 13);
}

void USART_SendData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len) {
    if(pHandle->TxState != USART_READY) return;
    pHandle->pTxBuffer = pBuffer;
    pHandle->TxLen = Len;
    pHandle->TxState = USART_BUSY_TX;
    pHandle->pUSARTx->CR1 |= (1 << 7); // Enable TXEIE
}

void USART_ReceiveData_IT(USART_Handle_t *pHandle, uint8_t *pBuffer, uint32_t Len) {
    if(pHandle->RxState != USART_READY) return;
    pHandle->pRxBuffer = pBuffer;
    pHandle->RxLen = Len;
    pHandle->RxState = USART_BUSY_RX;
    // Note: RXNEIE is already on from Init
}

void USART_IRQHandling(USART_Handle_t *pHandle) {
    uint32_t temp1, temp2;

    // A. ERROR CHECK
    if(pHandle->pUSARTx->SR & (1 << 3)) { // Overrun Error
        (void)pHandle->pUSARTx->DR; // Clear error
        if(pHandle->pCallback) pHandle->pCallback(USART_EVENT_ERROR);
    }

    // B. TXE LOGIC (Shoveling)
    temp1 = pHandle->pUSARTx->SR & (1 << 7);
    temp2 = pHandle->pUSARTx->CR1 & (1 << 7);
    if(temp1 && temp2) {
        pHandle->pUSARTx->DR = *pHandle->pTxBuffer++;
        if(--pHandle->TxLen == 0) {
            pHandle->pUSARTx->CR1 &= ~(1 << 7); // Stop TXE
            pHandle->pUSARTx->CR1 |= (1 << 6);  // Start TC
        }
    }

    // C. TC LOGIC (Silence check)
    temp1 = pHandle->pUSARTx->SR & (1 << 6);
    temp2 = pHandle->pUSARTx->CR1 & (1 << 6);
    if(temp1 && temp2) {
        pHandle->pUSARTx->CR1 &= ~(1 << 6); // Disable TCIE
        pHandle->TxState = USART_READY;
        if(pHandle->pCallback) pHandle->pCallback(USART_EVENT_TX_CMPLT);
    }

    // D. RXNE LOGIC (Doorbell)
    temp1 = pHandle->pUSARTx->SR & (1 << 5);
    temp2 = pHandle->pUSARTx->CR1 & (1 << 5);
    if(temp1 && temp2) {
        if(pHandle->RxState == USART_BUSY_RX) {
            *pHandle->pRxBuffer++ = (uint8_t)pHandle->pUSARTx->DR;
            if(--pHandle->RxLen == 0) {
                pHandle->RxState = USART_READY;
                if(pHandle->pCallback) pHandle->pCallback(USART_EVENT_RX_CMPLT);
            }
        } else {
            (void)pHandle->pUSARTx->DR; // Throw away
        }
    }
}