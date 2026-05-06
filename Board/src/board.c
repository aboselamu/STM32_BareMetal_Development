#include "board.h"
#include "usart_driver.h"
#include "pwm_driver.h"
#include "exti_driver.h"
#include "spi_driver.h"


void USART_MspInit(USART_Handle_t *pHandle) {
    if (pHandle->pUSARTx == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Clock
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

        // PA2, PA3 -> AF7
        GPIOA->MODER &= ~((3 << (2 * 2)) | (3 << (3 * 2)));
        GPIOA->MODER |=  ((2 << (2 * 2)) | (2 << (3 * 2)));
        GPIOA->AFR[0] |= ((7 << (2 * 4)) | (7 << (3 * 4)));

        NVIC_EnableIRQ(USART2_IRQn); // Switchboard Enable
    }
}

// for pwm port pin and mapping configuration 
void PWM_MspInit(PWM_HandleTypeDef *hpwm) {
    if (hpwm->Instance == TIM2) {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 Clock
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOA Clock

        // Configure PA5 -> Alternate Function 1 (TIM2_CH1)
        GPIOA->MODER &= ~(3U << (5 * 2)); // Clear bits for Pin 5
        GPIOA->MODER |=  (2U << (5 * 2)); // Set to Alternate Function
        
        // AFR[0] handles pins 0-7. Pin 5 uses bits 23:20.
        GPIOA->AFR[0] &= ~(0xFU << (5 * 4)); 
        GPIOA->AFR[0] |=  (1U << (5 * 4));   // Set AF1 (TIM2)
    }
}


//// for exti

void EXTI_MspInit(EXTI_Handle_t *pHandle) {
    // 1. Enable SYSCFG Clock (Required for mapping pins to lines)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // 2. Configure the GPIO Pin as Input
    if (pHandle->pPort == GPIOA) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        
        // Set as Input (00), Pull-Up (01)[cite: 12]
        pHandle->pPort->MODER &= ~(3U << (pHandle->line * 2));
        pHandle->pPort->PUPDR &= ~(3U << (pHandle->line * 2));
        pHandle->pPort->PUPDR |=  (1U << (pHandle->line * 2));
    }

    // 3. Enable NVIC IRQ for the specific line
    if (pHandle->line <= 4) {
        NVIC_EnableIRQ(EXTI0_IRQn + pHandle->line); 
    }
}


/* --- SPI MSP Implementation --- */
void SPI_MspInit(SPI_Handle_t *pHandle) {
    if (pHandle->pSPIx == SPI1) {
        // Mapping for SPI1: PA5 (CLK), PA6 (MISO), PA7 (MOSI)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
        
        GPIOA->MODER |= (2 << (2 * 5)) | (2 << (2 * 6)) | (2 << (2 * 7));
        GPIOA->AFR[0] |= (5 << (4 * 5)) | (5 << (4 * 6)) | (5 << (4 * 7));
        
        NVIC_EnableIRQ(SPI1_IRQn);
    }
}
