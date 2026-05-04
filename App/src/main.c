#include "board.h"
#include "spi_driver.h"
#include <stdio.h>

SPI_Handle_t hspi1;
uint8_t master_tx[] = "Hello ESP32!";
uint8_t master_rx[13];

void My_SPI_CpltCallback(SPI_Handle_t *hspi) {
    if (hspi->Instance == SPI1) {
        printf("Transfer Complete!\n");[cite: 13]
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
    }
}