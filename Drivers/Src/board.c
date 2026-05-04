#include "board.h"

static void GPIO_EnableClock(GPIO_TypeDef *port)
{
    if (port == GPIOA)      RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
}

void Board_GPIO_AF_Init(GPIO_Handle_t *gpio)
{
    if (gpio == NULL || gpio->port == NULL) return;
    
    GPIO_EnableClock(gpio->port);

    gpio->port->MODER &= ~(3U << (gpio->pin * 2));
    gpio->port->MODER |=  (2U << (gpio->pin * 2));

    if (gpio->pin < 8) {
        gpio->port->AFR[0] &= ~(0xFU << (gpio->pin * 4));
        gpio->port->AFR[0] |=  (gpio->af << (gpio->pin * 4));
    } else {
        uint8_t p = gpio->pin - 8;
        gpio->port->AFR[1] &= ~(0xFU << (p * 4));
        gpio->port->AFR[1] |=  (gpio->af << (p * 4));
    }
}