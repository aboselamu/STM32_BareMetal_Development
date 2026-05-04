#include "exti_driver.h"
#include "stm32f4xx.h"

void button_callback(uint8_t line) {
    // Toggle LED on PA5
    GPIOA->ODR ^= (1 << 5);
}

int main(void) {
    // 1. Enable GPIOA clock (both for LED and button)
    RCC->AHB1ENR |= (1 << 0);

    // 2. Configure PA5 as output (LED)
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |= (1 << (5 * 2));   // 01 = output
    GPIOA->OTYPER &= ~(1 << 5);       // push-pull
    GPIOA->OSPEEDR |= (3 << (5 * 2)); // high speed

    // 3. Configure PA0 as input with pull-up (button)
    GPIOA->MODER &= ~(3 << (0 * 2));  // input mode
    GPIOA->PUPDR &= ~(3 << (0 * 2));
    GPIOA->PUPDR |= (1 << (0 * 2));   // pull-up

    // 4. Configure EXTI for line 0 (PA0), rising edge
    exti_config_t cfg = {
        .line = 0,
        .trigger = 0,        // rising edge
        .gpio_port = GPIOA_BASE,
        .gpio_pin = 0
    };
    exti_init(&cfg);
    exti_set_callback(button_callback);

    // 5. Infinite loop – everything done in interrupt
    while (1);
}