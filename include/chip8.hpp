/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Defines the CPU's core specifications and other useful functions
 *****************************************************************************/

#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>

const unsigned int START_ADDRESS = 0x200;
const int MEM_SIZE_BYTES = 4096;
const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int VIDEO_WIDTH = 64;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int STACK_SIZE = 16;
const unsigned int N_REGISTERS = 16;
const unsigned int NUM_KEYS = 16;

const uint8_t fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Processor
{
  public:
    uint8_t keypad[NUM_KEYS]{};
    uint32_t video[VIDEO_WIDTH * VIDEO_HEIGHT]{};

    // Initialization
    Processor();
    uint8_t randGen();
    int load_rom(char* filename);
    void cycle();

  private:
    // Processor Data and Specifications
    uint8_t registers[N_REGISTERS]{};
    uint8_t memory[MEM_SIZE_BYTES]{};
    uint16_t index{};
    uint16_t pc{};
    uint16_t stack[STACK_SIZE]{};
    uint8_t stack_pointer{};
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    uint16_t opcode;

    // Opcodes
    void OP_00E0(); // Clear the display by zeroing out the video buffer
    void OP_00EE(); // Return a value
    void OP_1nnn(); // Jump to address nnn by setting pc to that address
    void OP_2nnn(); // Call subroutine at nnn
    void OP_3xnn(); // Skip instruction if register Vx = nn
    void OP_4xnn(); // Skip instruction if register Vx != nn
    void OP_5xy0(); // Skip instruction if register Vx == register Vy
    void OP_6xnn(); // Set register Vx = nn
    void OP_7xnn(); // Add nn to register x
    void OP_8xy0(); // Set register Vx to Vy;
    void OP_8xy1(); // Bitwise OR on registers Vx and Vy; stored in Vx
    void OP_8xy2(); // Bitwise AND on registers Vx and Vy; stored in Vx
    void OP_8xy3(); // Bitwise XOR on registers Vx and Vy; stored in Vx
    void OP_8xy4(); // Add two registers Vx and Vy store Vx, with a carry flag VF
    void OP_8xy5(); // Subtract two registers Vx and Vy store Vx, with borrow VF
    void OP_8xy6(); // Division by 2 and updating the flag
    void OP_8xy7(); // Subtract with borrow, Vx and Vy, store in Vx
    void OP_8xyE(); // Multiply Vx by 2
    void OP_9xy0(); // Skip next instruction if register Vx != register Vy

    void OP_Annn(); // Sets index to nnn
    void OP_Bnnn(); // Jump to location nnn + V0
    void OP_Cxnn(); // Set Vx = random byte AND kk
    void OP_Dxyn(); // Draws a sprite at coordinate (VX, VY) that has a width of 8
                    // pixels and a height of N pixels

    void OP_Ex9e(); // Skip next instruction if key == Vx is pressed.
    void OP_Exa1(); // Skip next instruction if key with the value of Vx is not
                    // pressed.

    void OP_Fx07(); // Set register Vx to delay timer.
    void OP_Fx0a(); // Wait for keypress and set register Vx to that value.
    void OP_Fx15(); // Set delay timer to register Vx.
    void OP_Fx18(); // Set sound timer to register Vx.
    void OP_Fx1e(); // Set index += register Vx.
    void OP_Fx29(); // Store fontdata in index
    void OP_Fx33(); // Store BCD representation of Vx in memory I, I+1 ...

    void OP_Fx55(); // Store registers V0 to Vx in memory from index onwards
    void OP_Fx65(); // Read registers V0 to Vx in memory from index onwards
    void OP_NULL(); // Default, do nothing if instruction cannot be understood.

    void Table0()
    {
        ((*this).*(table0[opcode & 0x000Fu]))();
    }

    void Table8()
    {
        ((*this).*(table8[opcode & 0x000Fu]))();
    }

    void TableE()
    {
        ((*this).*(tableE[opcode & 0x000Fu]))();
    }

    void TableF()
    {
        uint8_t index = opcode & 0x00FFu;
        if (index > 0x65)
            index = 0x00; // default to OP_NULL
        ((*this).*(tableF[index]))();
    }

    typedef void (Processor::*ProcessorFunc)();
    ProcessorFunc table[0xF + 1];
    ProcessorFunc table0[0xE + 1];
    ProcessorFunc table8[0xE + 1];
    ProcessorFunc tableE[0xE + 1];
    ProcessorFunc tableF[0x65 + 1];
};
