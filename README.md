# STM32 Bare-Metal Firmware Driver Library

## 🧠 Overview
This project is a custom-built **bare-metal firmware framework** for STM32F4 microcontrollers written in C, with no dependency on HAL or high-level libraries.

It demonstrates low-level embedded systems design, including:
- Interrupt-driven firmware architecture
- Peripheral driver development (USART, PWM, EXTI, SPI)
- Real-time hardware control
- Modular firmware structure (drivers + applications separation)

A fully working example system is included:
👉 Remote LED Dimmer controlled via UART commands

---

## 🎯 Key Objectives
- Develop a deeper understanding of STM32 internal peripherals
- Build reusable, scalable embedded drivers
- Implement interrupt-based real-time systems
- Practice register-level programming and clock configuration

---

## 🏗 Project Structure


This structure follows a modular firmware design approach similar to industry embedded systems.

---

## 🎛 Featured Project: Remote LED Dimmer System

A real-time embedded system that controls LED brightness using UART commands.

### 🔧 Features
- USART2 interrupt-based input handling (non-blocking)
- PWM control using TIM2 Channel 1
- ISR-to-main loop communication using volatile flags
- 5-level brightness control (commands '0' → '4')

### ⚙️ How It Works
1. User sends a character via UART terminal ('0'–'4')
2. USART interrupt captures input asynchronously
3. Main loop processes command without blocking CPU
4. PWM duty cycle is updated in real time

---

## 🧠 Embedded Concepts Demonstrated

### 🔹 Interrupt-Driven Design
- USART RX handled via ISR
- Non-blocking firmware execution

### 🔹 PWM Signal Generation
- Timer-based PWM using STM32 TIM2
- Prescaler and Auto-Reload register configuration
- Preload enabled for glitch-free updates

### 🔹 State Management
- Uses `volatile` flags for ISR-to-main communication
- Simple embedded state machine (READY/BUSY logic)

### 🔹 Efficient Computation
- Fixed-point arithmetic used for duty cycle scaling
- Avoids floating-point overhead on MCU

---

## ⚙️ Peripheral Drivers

### USART Driver
- Interrupt-based RX handling
- Non-blocking transmission design
- Overrun error handling (ORE recovery)
- Multi-instance support structure

### PWM Driver
- Manual timer configuration (PSC / ARR)
- Hardware-synchronized updates
- Stable waveform generation

### EXTI Driver
- GPIO-to-interrupt mapping via SYSCFG
- Configurable edge detection (rising/falling/both)
- NVIC interrupt integration

---

## 🚀 How to Run

### Hardware Setup
- LED connected to: **PA0 (TIM2_CH1)**
- UART connection: **PA2 / PA3 (USART2)**

### Software
- Compile using STM32CubeIDE or GCC toolchain
- Flash to STM32F4 board

### Usage
1. Open serial terminal at **9600 baud**
2. Send characters:
   - `0` → LED OFF
   - `1–4` → Increasing brightness levels

---

## 📌 Learning Outcome
This project demonstrates practical understanding of:
- Bare-metal embedded system design
- Peripheral driver development
- Interrupt handling architecture
- Real-time system behavior on STM32

---

## 🚧 Future Improvements
- Add DMA-based USART communication
- Expand driver library (SPI, I2C)
- Add sensor integration projects
- Implement RTOS version for comparison

<img width="760" height="536" alt="Capture" src="https://github.com/user-attachments/assets/098e4182-d1f1-4ff3-b950-eed4c85e6d87" />

