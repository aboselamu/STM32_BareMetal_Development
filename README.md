# STM32 Bare-Metal Driver Library (Final)

### 🎯 Purpose
This repository showcases a custom-built Hardware Abstraction Layer (HAL) for the STM32F4 series. It demonstrates a deep understanding of memory mapping, clock trees, and real-time control.

### 🛠 Project Status
- [x] **EXTI Driver:** External interrupt signal chain mapping[cite: 1].
- [x] **USART Driver:** Interrupt-driven asynchronous communication[cite: 6, 7].
- [x] **PWM Driver:** Precise signal generation using Multi-channel Timers.

---

## 🧬 Featured: PWM Driver (`pwm_driver.c`)
Demonstrates the use of General Purpose Timers to generate PWM signals for motor or LED control.

* **Time-Base Calculation:** Manual configuration of Prescaler (PSC) and Auto-Reload (ARR).
    * $$Frequency = \frac{f_{CK}}{(PSC + 1) \times (ARR + 1)}$$
* **Preload Management:** Enabled **Auto-reload Preload (ARPE)** to synchronize register updates with timer overflows, ensuring glitch-free operation.
* **Board Abstraction:** Features a dedicated `Board_GPIO_AF_Init` to handle Alternate Function multiplexing independently from the PWM logic[cite: 8, 9].

---

## 🧬 Featured: USART Driver (`usart_driver.c`)
* **State Management:** Implemented `READY` and `BUSY` flags to prevent resource contention during background transfers[cite: 7].
* **Interrupt Decoupling:** Generic IRQ handling allows the same driver to support multiple USART instances[cite: 4, 5, 6].

---

### 🔬 Core Technical Focus
* **Bit-Manipulation:** Direct register control for zero-overhead performance[cite: 1, 6].
* **Manual Clock Trees:** Explicit peripheral clock management via RCC[cite: 1, 6, 8].