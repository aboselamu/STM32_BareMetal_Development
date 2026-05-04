#include "usart_driver.h"

// External handle defined in main.c
extern USART_Handle_t myUsart2Handle;

/**
 * @brief This is the actual function name the CPU looks for.
 * It is defined in the Startup (.s) file.
 */
void USART2_IRQHandler(void) {
    // We simply redirect the interrupt to our generic driver function
    USART_IRQHandling(&myUsart2Handle);
}