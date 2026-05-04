#include "exti_driver.h"
#include "stm32f4xx.h"

#define SYSCFG_EN_BIT   14
#define GPIOA_BASE      0x40020000
#define GPIO_OFFSET     0x400

static void (*user_callback)(uint8_t) = 0;

static uint8_t get_port_number(uint32_t port_addr) {
    return (port_addr - GPIOA_BASE) / GPIO_OFFSET;
}

void exti_init(const exti_config_t *cfg) {
    // 1. Enable SYSCFG clock
    RCC->APB2ENR |= (1 << SYSCFG_EN_BIT);
    

    /* Refer to RM0090: SYSCFG_EXTICR1-4 logic for mapping Port x to Line y */
    // 2. Connect GPIO pin to EXTI line via SYSCFG_EXTICR
    uint8_t reg_idx = cfg->line / 4;
    uint8_t shift = (cfg->line % 4) * 4;
    uint8_t port_num = get_port_number(cfg->gpio_port);
    SYSCFG->EXTICR[reg_idx] &= ~(0xF << shift);
    SYSCFG->EXTICR[reg_idx] |= (port_num << shift);

    // 3. Unmask the interrupt in EXTI_IMR
    EXTI->IMR |= (1 << cfg->line);

    // 4. Configure trigger edge(s)
    // Clear both edges first (optional)
    EXTI->RTSR &= ~(1 << cfg->line);
    EXTI->FTSR &= ~(1 << cfg->line);
    if (cfg->trigger == 0) {          // rising
        EXTI->RTSR |= (1 << cfg->line);
    } else if (cfg->trigger == 1) {   // falling
        EXTI->FTSR |= (1 << cfg->line);
    } else if (cfg->trigger == 2) {   // both
        EXTI->RTSR |= (1 << cfg->line);
        EXTI->FTSR |= (1 << cfg->line);
    }

    // 5. Clear any pending flag
    EXTI->PR |= (1 << cfg->line);

    // 6. Enable NVIC for the appropriate IRQ (i implemented for only lines 0-4 for simplicity)
    switch (cfg->line) {
        case 0: NVIC_EnableIRQ(EXTI0_IRQn); break;
        case 1: NVIC_EnableIRQ(EXTI1_IRQn); break;
        case 2: NVIC_EnableIRQ(EXTI2_IRQn); break;
        case 3: NVIC_EnableIRQ(EXTI3_IRQn); break;
        case 4: NVIC_EnableIRQ(EXTI4_IRQn); break;
        // For lines 5-9 and 10-15 you would need shared handlers
        default: break;
    }
}

void exti_set_callback(void (*callback)(uint8_t line)) {
    user_callback = callback;
}

// EXTI0 interrupt handler (dedicated for line 0 just only one line implementation for simplicity)
void EXTI0_IRQHandler(void) {
    // Clear the pending flag
    EXTI->PR = (1 << 0);
    if (user_callback) user_callback(0);
}