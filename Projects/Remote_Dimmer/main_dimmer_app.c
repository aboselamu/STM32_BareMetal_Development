#include "stm32f4xx.h"

// --- 1. Global Section ---
// These MUST be volatile because the hardware/ISR changes them
volatile uint8_t rx_data = 0;
volatile uint8_t char_received_flag = 0;

// --- 2. Hardware Initialization ---

void Clock_Config(void) {
    // Enable GPIOA clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    // Enable USART2 and TIM2 clocks (both on APB1)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void GPIO_Config(void) {
    /* 
       PA0 -> TIM2_CH1 (AF1)
       PA2 -> USART2_TX (AF7) - Optional but good for feedback
       PA3 -> USART2_RX (AF7)
    */
    
    // Reset and Set MODER to Alternate Function (binary 10)
    GPIOA->MODER &= ~((3 << (0 * 2)) | (3 << (2 * 2)) | (3 << (3 * 2)));
    GPIOA->MODER |=  ((2 << (0 * 2)) | (2 << (2 * 2)) | (2 << (3 * 2)));

    // Set Alternate Function Low Register (AFRL)
    // AF1 for PA0 (0001)
    GPIOA->AFR[0] &= ~(0xF << (0 * 4));
    GPIOA->AFR[0] |=  (1 << (0 * 4));

    // AF7 for PA2 and PA3 (0111)
    GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFR[0] |=  ((7 << (2 * 4)) | (7 << (3 * 4)));
}

void USART2_Config(void) {
    // Baud rate calculation for 9600 @ 16MHz
    // 16,000,000 / (16 * 9600) = 104.166
    // Mantissa = 104 (0x68), Fraction = 0.166 * 16 = 2.6 -> 3 (0x3)
    USART2->BRR = 0x0683;

    // CR1: Enable RE (bit 2), RXNEIE (bit 5), and UE (bit 13)
    USART2->CR1 |= USART_CR1_RE | USART_CR1_RXNEIE | USART_CR1_UE;

    // Enable USART2 Interrupt in NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}

void TIM2_PWM_Config(void) {
    /*
       Goal: 1kHz frequency
       Formula: $$f = \frac{f_{clk}}{(PSC+1)(ARR+1)}$$
       16,000,000 / (16 * 1000) = 1000 total counts
    */
    TIM2->PSC = 15;        // Divide 16MHz by 16 = 1MHz timer clock
    TIM2->ARR = 999;       // 1000 counts = 1ms period (1kHz)
    TIM2->CCR1 = 0;        // Start with LED OFF

    // Output Compare Mode: PWM Mode 1 (110) on Channel 1
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // Preload Enable

    // Enable Channel 1 Output
    TIM2->CCER |= TIM_CCER_CC1E;

    // Enable Counter
    TIM2->CR1 |= TIM_CR1_CEN;
}

// --- 3. The ISR (Texas Ranger) ---

void USART2_IRQHandler(void) {
    // Check if the RX Not Empty flag is set
    if (USART2->SR & USART_SR_RXNE) {
        // Reading the Data Register automatically clears the RXNE flag
        rx_data = (uint8_t)(USART2->DR & 0xFF);
        char_received_flag = 1;
    }
}

// --- 4. The Main Loop ---

int main(void) {
    Clock_Config();
    GPIO_Config();
    USART2_Config();
    TIM2_PWM_Config();

    while (1) {
        if (char_received_flag) {
            // Robustness: Only process '0' through '4'
            if (rx_data >= '0' && rx_data <= '4') {
                uint8_t level = rx_data - '0'; // Convert ASCII to Integer
                
                // Map 0-4 to 0-100% of ARR (999)
                // Duty cycle calculation: (level * 25) percent of 999
                TIM2->CCR1 = (level * 999) / 4;
            }
            
            // Clear the software flag
            char_received_flag = 0;
        }
    }
}