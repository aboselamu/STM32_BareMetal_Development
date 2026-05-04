# STM32 Bare-Metal Development Library

### 🎯 Purpose
This repository showcases a custom-built **Hardware Abstraction Layer (HAL)** and integrated system projects for the STM32F4 series. It demonstrates a deep understanding of memory mapping, clock trees, and real-time hardware control by interacting directly with silicon registers[cite: 1].

---

### 🛠 Repository Structure
*   **`Drivers/`**: Reusable, handle-based peripheral drivers (USART, PWM, EXTI) designed for modularity.
*   **`Projects/`**: System-level applications that integrate multiple drivers to solve real-world hardware tasks.
*   **`App/`**: Independent test files for verifying individual driver functionality.

---

### 🚀 Featured Project: Remote LED Dimmer
Located in `Projects/Remote_Dimmer/main_dimmer_app.c`, this application integrates communication and timing peripherals into a functional "product".

*   **Asynchronous Control**: Uses a **USART2 receive interrupt** to capture user commands ('0'-'4') without stalling the processor.
*   **State Bridging**: Implements a `volatile` software flag to pass data safely from the Interrupt Service Routine (ISR) to the main application loop.
*   **Real-Time Actuation**: Converts ASCII inputs into integer duty-cycle levels for **TIM2 Channel 1**, providing 5 levels of LED brightness.
*   **Embedded Optimization**: Utilizes fixed-point scaling `(level * 999) / 4` to maintain high-speed arithmetic without the overhead of a floating-point unit.

---

### 🧬 Peripheral Driver Highlights

#### **PWM Driver** (`pwm_driver.c`)
*   **Time-Base Calculation**: Manual configuration of Prescaler (PSC) and Auto-Reload (ARR) to define signal frequency.
    *   $$Frequency = \frac{f_{CK}}{(PSC + 1) \times (ARR + 1)}$$
*   **Preload Management**: Enabled **Auto-reload Preload (ARPE)** and Output Compare Preload to ensure that updates to duty cycle (CCR) and period (ARR) are synchronized with timer overflows, preventing signal glitches.
*   **Board Abstraction**: Features a dedicated `Board_GPIO_AF_Init` to handle Alternate Function multiplexing independently from the timer logic.

#### **USART Driver** (`usart_driver.c`)
*   **State Management**: Implemented `READY` and `BUSY` flags within a non-blocking state machine to manage data transfers.
*   **Interrupt Decoupling**: Generic IRQ handling allows the same driver logic to support multiple USART instances across the chip.
*   **Error Resilience**: Detects and clears **Overrun Errors (ORE)** to prevent peripheral lock-up.

#### **EXTI Driver** (`exti_driver.c`)
*   **Signal Chain**: Maps physical GPIO pins to the NVIC using the `SYSCFG_EXTICR` registers[cite: 1].
*   **Configurable Triggers**: Supports Rising, Falling, or Both-edge detection for external hardware interrupts[cite: 1].

---

### 🔬 Core Technical Focus
*   **Bit-Manipulation**: Direct register control using bitwise operators (OR/AND/XOR) for zero-overhead performance[cite: 1].
*   **Manual Clock Management**: Explicit management of the STM32 clock tree (AHB/APB buses) via RCC registers before peripheral initialization[cite: 1].

---

### 🚀 To Setup and Run:
1.  **Hardware**: Wire your LED to **PA0** (TIM2_CH1) and your Serial Debugger to **PA2/PA3** (USART2).
2.  **Compile**: Use the source code located in `Projects/Remote_Dimmer/main_dimmer_app.c`.
3.  **Command**: Open a terminal at **9600 baud** and send characters **'0'** through **'4'** to adjust brightness.