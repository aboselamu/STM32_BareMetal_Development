# STM32 Bare-Metal Driver Library

### 🎯 Purpose
This repository showcases a custom-built Hardware Abstraction Layer (HAL) for the STM32F4 series, written from scratch using CMSIS register definitions. It demonstrates a deep understanding of peripheral memory mapping, clock trees, and interrupt-driven firmware design.

### 🛠 Project Status
- [x] **EXTI Driver:** Completed & Documented
- [x] **USART Driver:** Completed & Documented (Interrupt-Driven)
- [ ] **SPI Driver:** Upload Pending
- [ ] **PWM/Timers:** Upload Pending

---

## 🧬 Featured: USART Driver (`usart_driver.c`)
This driver implements a high-performance, asynchronous serial communication interface using a **non-blocking (Interrupt-based)** architecture[cite: 6].

### Technical Highlights:
*   **State Machine Management:** Utilizes `USART_READY`, `USART_BUSY_TX`, and `USART_BUSY_RX` states to ensure thread-safe data handling and prevent resource contention[cite: 6, 7].
*   **Two-Stage Transmission Logic:** Manages the `TXE` (Transmit Data Register Empty) and `TC` (Transmission Complete) flags to ensure high throughput while maintaining protocol integrity[cite: 4, 6].
*   **Interrupt Decoupling:** Implements a generic `USART_IRQHandling` function. This allows the driver to be reused across any USART instance (USART1, USART2, etc.) by passing a handle from the specific Vector Address[cite: 5, 6].
*   **Error Hygiene:** Automatically detects and clears **Overrun Errors (ORE)** by performing a synchronized sequence of register reads, preventing peripheral lock-up during high-speed transfers[cite: 4, 6].

### How it Works:
1.  **Init:** Configures Baud Rate, Word Length, and Stop bits while enabling the "Front Gate" in the NVIC[cite: 6, 7].
2.  **Transfer:** The application calls `USART_SendData_IT`. The CPU does not wait; it continues executing other code while the driver "shovels" data in the background via interrupts[cite: 6].
3.  **Callback:** Once the transfer is complete, the driver triggers a user-defined callback function[cite: 4, 7].

---

## 🧬 Featured: EXTI Driver (`exti_driver.c`)
Demonstrates the full interrupt signal chain from physical pins to the processor core.

### Technical Highlights:
*   **SYSCFG Multiplexing:** Dynamic port-to-line mapping via `SYSCFG_EXTICR`[cite: 1].
*   **Trigger Logic:** Configurable edge detection (Rising/Falling/Both) using `RTSR` and `FTSR`[cite: 1, 2].

---

### 🔬 Technical Focus
*   **Bit-Masking:** Direct register manipulation (OR/AND/XOR) for zero-overhead configuration.
*   **Manual Clock Management:** Explicit peripheral clock enablement via the RCC (Reset and Clock Control) registers.