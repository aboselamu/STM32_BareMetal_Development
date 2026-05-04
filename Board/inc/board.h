#ifndef BOARD_H
#define BOARD_H

#include "usart_driver.h"
#include "spi_driver.h"
#include "exti_driver.h"

/* Prototypes for ALL peripheral hardware setups */
void USART_MspInit(USART_Handle_t *pHandle);
void PWM_MspInit(PWM_Handle_t *pHandle);
void SPI_MspInit(SPI_Handle_t *pHandle);
void EXTI_MspInit(uint8_t pin_number);

#endif