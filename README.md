# 🐍 Snake Game on AVR with LCD

This is a simple implementation of the classic Snake Game developed using AVR microcontroller and an LCD display. The game runs without using any operating system or interrupts — it is built purely using polling and delay-based timing.

---

## 🎮 Features

- Runs on AVR microcontroller (e.g., ATmega series)
- Display output on an alphanumeric or graphical LCD
- Game logic written in C (no OS or RTOS)
- Controlled using basic input buttons
- Self-contained timing logic using delay functions (no interrupts)

---

## 🧠 How It Works

- The snake moves in one direction until user input changes it
- If the snake hits the wall or itself, the game ends
- Food appears randomly and increases the snake's length
- LCD is updated frame by frame through a refresh loop

---

## 🧰 Technologies Used

- **Microcontroller**: AVR (e.g., ATmega16/32)
- **Programming Language**: C
- **Display**: 16x2 or 128x64 LCD (depending on your setup)
- **Toolchain**: AVR-GCC, Atmel Studio or PlatformIO
- **Programmer**: USBasp or similar

---
## 📷 Screenshots
![WhatsApp Image 2025-08-06 at 20 13 45_67ee8bb7](https://github.com/user-attachments/assets/75668376-7d99-47aa-98c9-91e624148273)



## 🚀 Getting Started

1. Clone the repository:
   ```bash
   git clone 'https://github.com/ahmeddmm/Snake-Game.git'

