# STM32 Bare-Metal Driver Library

### 🎯 Purpose
This repository showcases a custom-built Hardware Abstraction Layer (HAL) for the STM32F4 series, written entirely from scratch using CMSIS register definitions. 

### 🛠 Implemented Features
* **SPI Driver:** Full-duplex communication using the Circular Shift Register logic. Configured for Master Mode with Push-Pull output.
* **USART Driver:** Asynchronous data transfer with custom Baud Rate Calculation logic ($Baud = \frac{f_{CK}}{8 \times (2 - OVER8) \times USARTDIV}$).
* **EXTI Driver:** Configured the NVIC and SYSCFG registers to handle external interrupts for real-time sensing.

### 🔬 Technical Focus
* **Bit-Masking:** All peripheral configurations are done via direct bit-manipulation (OR/AND/XOR).
* **Clock Control:** Manual enablement of peripheral clocks via the RCC (Reset and Clock Control) register.