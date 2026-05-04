void USART_IRQHandling(USART_Handle_t *pHandle) {
    uint32_t temp1, temp2;

    // --- 1. ERROR HANDLING ---
    // Check if ORE (Overrun) or FE (Framing Error) occurred
    if(pHandle->pUSARTx->SR & ((1 << 3) | (1 << 1))) {
        // Clear errors by reading SR then DR
        (void)pHandle->pUSARTx->SR;
        (void)pHandle->pUSARTx->DR;
        if(pHandle->pCallback) pHandle->pCallback(USART_EVENT_ERROR);
        return; 
    }

    // --- 2. TRANSMIT LOGIC (TXE) ---
    temp1 = pHandle->pUSARTx->SR & (1 << 7);
    temp2 = pHandle->pUSARTx->CR1 & (1 << 7);
    if(temp1 && temp2) {
        pHandle->pUSARTx->DR = *pHandle->pTxBuffer++;
        if(--pHandle->TxLen == 0) {
            pHandle->pUSARTx->CR1 &= ~(1 << 7); // Stop Shoveling
            pHandle->pUSARTx->CR1 |= (1 << 6);  // Wait for Silence (TC)
        }
    }

    // --- 3. TRANSMIT COMPLETE (TC) ---
    temp1 = pHandle->pUSARTx->SR & (1 << 6);
    temp2 = pHandle->pUSARTx->CR1 & (1 << 6);
    if(temp1 && temp2) {
        pHandle->pUSARTx->CR1 &= ~(1 << 6); // Close TC gate
        pHandle->TxState = USART_READY;
        if(pHandle->pCallback) pHandle->pCallback(USART_EVENT_TX_CMPLT);
    }

    // --- 4. RECEIVE LOGIC (RXNE) ---
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
            (void)pHandle->pUSARTx->DR; // Throw away unrequested data
        }
    }
}