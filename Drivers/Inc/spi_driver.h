#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include "stm32f4xx.h" 

/* ================= TYPES ================= */
typedef enum {
    SPI_READY = 0,
    SPI_BUSY_TX_RX,
    SPI_ERROR_TIMEOUT,
    SPI_ERROR_OVR,
    SPI_ERROR_MODF
} SPI_State_t;

typedef struct {
    uint8_t mode;
    uint8_t bus_config;
    uint8_t prescaler;
    uint8_t data_size;
    uint8_t cpol;
    uint8_t cpha;
    uint8_t nss;
} SPI_Config_t;

typedef struct SPI_Handle_s {
    SPI_TypeDef *Instance;   /* Register base address (e.g., SPI1) 
    SPI_Config_t Config;
    uint8_t *pTxBuffer;
    uint8_t *pRxBuffer;
    uint32_t TxLen;
    uint32_t RxLen;
    uint32_t TxCount;
    uint32_t RxCount;
    SPI_State_t State;

    /* Callbacks */
    void (*TxRxCpltCallback)(struct SPI_Handle_s *hspi);
    void (*ErrorCallback)(struct SPI_Handle_s *hspi);  
} SPI_Handle_t;

/* ================= API ================= */
void SPI_Init(SPI_Handle_t *hspi);
uint8_t SPI_Transfer_IT(SPI_Handle_t *hspi, uint8_t *tx, uint8_t *rx, uint32_t len);
void SPI_IRQHandling(SPI_Handle_t *hspi);

/* MSP Hook - To be implemented in board.c */
void SPI_MspInit(SPI_Handle_t *hspi);

#endif
