#include "exti_driver.h"

// Private storage for handles to be used in ISR
static EXTI_Handle_t *gpEXTI_Handles[16]; 

void EXTI_Init(EXTI_Handle_t *pHandle) {
    // 1. Trigger the Board-specific setup (Clock and Pin mode)[cite: 10]
    EXTI_MspInit(pHandle);
    
    // Store handle for the ISR to use
    gpEXTI_Handles[pHandle->line] = pHandle;

    // 2. Map Port to EXTI Line via SYSCFG[cite: 10]
    // Each EXTICR register holds 4 lines, each 4 bits wide.
    uint8_t reg_idx = pHandle->line / 4;
    uint8_t shift = (pHandle->line % 4) * 4;
    
    // Get port code (A=0, B=1, etc.)
    uint8_t port_code = ((uint32_t)pHandle->pPort - GPIOA_BASE) / 0x400;[cite: 10]
    
    SYSCFG->EXTICR[reg_idx] &= ~(0xFU << shift);
    SYSCFG->EXTICR[reg_idx] |= (port_code << shift);

    // 3. Configure Trigger Edges[cite: 10]
    EXTI->RTSR &= ~(1U << pHandle->line);
    EXTI->FTSR &= ~(1U << pHandle->line);

    if (pHandle->trigger == EXTI_TRIGGER_RISING || pHandle->trigger == EXTI_TRIGGER_BOTH)
        EXTI->RTSR |= (1U << pHandle->line);
    if (pHandle->trigger == EXTI_TRIGGER_FALLING || pHandle->trigger == EXTI_TRIGGER_BOTH)
        EXTI->FTSR |= (1U << pHandle->line);

    // 4. Unmask the interrupt[cite: 10]
    EXTI->IMR |= (1U << pHandle->line);
}

void EXTI_IRQHandling(uint8_t line) {
    // Clear the pending bit by writing 1 to it[cite: 10]
    if (EXTI->PR & (1U << line)) {
        EXTI->PR = (1U << line);
        
        // Call the application's logic hook
        if (gpEXTI_Handles[line] && gpEXTI_Handles[line]->pApplicationCallback) {
            gpEXTI_Handles[line]->pApplicationCallback(line);[cite: 10]
        }
    }
}