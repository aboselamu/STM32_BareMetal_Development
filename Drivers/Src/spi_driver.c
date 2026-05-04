#include "spi_driver.h"

/* Static internal helpers (No spi_private.h needed!) */[cite: 12]
static void SPI_TXE_Handler(SPI_Handle_t *hspi);
static void SPI_RXNE_Handler(SPI_Handle_t *hspi);
static void SPI_CloseTransfer(SPI_Handle_t *hspi);
static void SPI_ClearOVR(SPI_Handle_t *hspi);

__weak void SPI_MspInit(SPI_Handle_t *hspi) {
    (void)hspi;
}

void SPI_Init(SPI_Handle_t *hspi) {
    /* 1. Trigger Board-level setup (Clock and Pins) */
    SPI_MspInit(hspi);

    /* 2. Configure CR1 */
    uint32_t temp = 0;
    temp |= (hspi->Config.mode << SPI_CR1_MSTR_Pos);
    temp |= (hspi->Config.prescaler << SPI_CR1_BR_Pos);
    temp |= (hspi->Config.cpol << SPI_CR1_CPOL_Pos);
    temp |= (hspi->Config.cpha << SPI_CR1_CPHA_Pos);
    temp |= (hspi->Config.data_size << SPI_CR1_DFF_Pos);
    temp |= (hspi->Config.nss << SPI_CR1_SSM_Pos);[cite: 10]

    hspi->Instance->CR1 = temp;

    /* 3. Enable SPI */
    hspi->Instance->CR1 |= SPI_CR1_SPE;[cite: 10]
    hspi->State = SPI_READY;
}

uint8_t SPI_Transfer_IT(SPI_Handle_t *hspi, uint8_t *tx, uint8_t *rx, uint32_t len) {
    if (hspi->State != SPI_READY || len == 0) return 0;

    hspi->State = SPI_BUSY_TX_RX;
    hspi->pTxBuffer = tx;
    hspi->pRxBuffer = rx;
    hspi->TxLen = len;
    hspi->RxLen = len;
    hspi->TxCount = 0;
    hspi->RxCount = 0;

    /* Enable interrupts */
    hspi->Instance->CR2 |= (SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);[cite: 10]
    return 1;
}

void SPI_IRQHandling(SPI_Handle_t *hspi) {
    uint32_t sr = hspi->Instance->SR;
    uint32_t cr2 = hspi->Instance->CR2;

    if ((sr & SPI_SR_TXE) && (cr2 & SPI_CR2_TXEIE)) SPI_TXE_Handler(hspi);
    if ((sr & SPI_SR_RXNE) && (cr2 & SPI_CR2_RXNEIE)) SPI_RXNE_Handler(hspi);

    if ((sr & SPI_SR_OVR) && (cr2 & SPI_CR2_ERRIE)) {
        SPI_ClearOVR(hspi);
        hspi->State = SPI_ERROR_OVR;
        if (hspi->ErrorCallback) hspi->ErrorCallback(hspi);
        SPI_CloseTransfer(hspi);
    }
    
    if (sr & SPI_SR_MODF) {
        hspi->State = SPI_ERROR_MODF;
        if (hspi->ErrorCallback) hspi->ErrorCallback(hspi);
        SPI_CloseTransfer(hspi);
    }
}[cite: 10]

/* ... Keep your existing SPI_TXE_Handler, SPI_RXNE_Handler, 
   SPI_CloseTransfer, and SPI_ClearOVR exactly as you wrote them ... */