# STM32 Bare-Metal Driver Library

### 🎯 Purpose
This repository showcases a custom-built Hardware Abstraction Layer (HAL) for the STM32F4 series, written from scratch using CMSIS register definitions to demonstrate deep-level hardware understanding.

### 🛠 Project Status
- [x] **EXTI Driver:** Completed & Documented
- [ ] **USART Driver:** Upload Pending
- [ ] **SPI Driver:** Upload Pending

---

### 🧬 Featured Driver: EXTI (External Interrupts)
Located in: `Drivers/Src/exti_driver.c` and `Drivers/Inc/exti_driver.h`[cite: 1, 2]

This implementation demonstrates the full interrupt signal chain on the STM32F4:
1. **SYSCFG Mapping:** Dynamically connects GPIO pins to EXTI lines using `SYSCFG_EXTICR` registers.
2. **Edge Detection:** Configures Rising, Falling, or Both-edge triggers via `RTSR` and `FTSR`[cite: 1, 2].
3. **NVIC Integration:** Direct management of the Nested Vectored Interrupt Controller for lines 0-4[cite: 1].
4. **Callback System:** Uses function pointers to allow `main_3.c` to handle button presses without modifying driver code[cite: 1, 3].

### 🔬 Technical Focus
* **Bit-Masking:** All configurations are done via direct bit-manipulation (OR/AND/XOR) to maintain maximum efficiency.
* **Clock Control:** Manual enablement of peripheral clocks (like SYSCFG) via the RCC register[cite: 1].