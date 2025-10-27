# CHIP-8 Emulator

A modern C++ implementation of the classic **CHIP-8 virtual machine**, featuring SDL2-based graphics and keyboard input. Created by Soham Dhar.  
Designed with **clean architecture**, cross-platform compatibility, and modern C++ practices in mind.  

---

## Features

- Full CHIP-8 opcode support (00E0 – FX65)  
- SDL2 graphics output (64×32 resolution, scalable)  
- Keyboard input mapped to standard CHIP-8 layout  
- Cross-platform: Linux, macOS, Windows (MSYS2 / MinGW / Visual Studio)  
- Debug and release build configurations  
- Clean out-of-source build structure (`src/`, `include/`, `build/`, `bin/`)  

---

## Project Structure
```
chip8/
├── bin/          # Executable output
├── build/        # Build artifacts and object files
├── include/      # Public header files
│   ├── chip8.hpp
│   └── Platform.hpp
├── src/          # Source files (.cpp)
│   ├── main.cpp
│   ├── Platform.cpp
│   ├── chip8.cpp
│   └── opcodes.cpp
├── roms/         # Optional: Store .ch8 ROMs here
├── Makefile      # Build script
└── README.md     # Project documentation
```
---

## Prerequisites

- **C++17** compatible compiler  
- **SDL2** development libraries  

### Linux

- Ubuntu / Debian
```bash
sudo apt update
sudo apt install g++ libsdl2-dev
```

- Arch / Manjaro
```bash
sudo pacman -Syu gcc sdl2
```

### macOS (Homebrew)
```bash
brew install sdl2
```

### Windows (MSYS2 / MinGW: Use pacman inside MSYS2 only)
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2
```
---

## Controls
```
| Original CHIP-8 | Keyboard |
|:---------------:|:--------:|
| 1 2 3 C         | 1 2 3 4  |
| 4 5 6 D         | Q W E R  |
| 7 8 9 E         | A S D F  |
| A 0 B F         | Z X C V  |
```
---

## Usage
```bash
mkdir roms
```
Copy your .ch8 file into roms.
```
./bin/chip8 10 10 roms/<YourROM>.ch8
```
---

## License
This project is licensed under the MIT License.
