/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Implements all the OPcodes
 *****************************************************************************/

#include "chip8.hpp"
#include <cstring>
#include <iostream>

void Processor::OP_00E0()
{
    std::memset(video, 0, sizeof(video));
}

void Processor::OP_00EE()
{
    if (stack_pointer == 0)
        return;
    stack_pointer--;
    pc = stack[stack_pointer];
}

void Processor::OP_1nnn()
{
    pc = opcode & 0x0FFFu;
}

void Processor::OP_2nnn()
{
    if (stack_pointer >= STACK_SIZE)
        return;
    stack[stack_pointer++] = pc;
    pc = opcode & 0x0FFFu;
}

void Processor::OP_3xnn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t nn = opcode & 0x00FFu;
    if (registers[x] == nn)
        pc += 2;
}

void Processor::OP_4xnn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t nn = opcode & 0x00FFu;
    if (registers[x] != nn)
        pc += 2;
}

void Processor::OP_5xy0()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    if (registers[x] == registers[y])
        pc += 2;
}

void Processor::OP_6xnn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    registers[x] = opcode & 0x00FFu;
}

void Processor::OP_7xnn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    registers[x] += opcode & 0x00FFu;
}

void Processor::OP_8xy0()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[x] = registers[y];
}

void Processor::OP_8xy1()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[x] |= registers[y];
}

void Processor::OP_8xy2()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[x] &= registers[y];
}

void Processor::OP_8xy3()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[x] ^= registers[y];
}

void Processor::OP_8xy4()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    uint16_t sum = registers[x] + registers[y];
    registers[0xF] = sum > 0xFF;
    registers[x] = static_cast<uint8_t>(sum);
}

void Processor::OP_8xy5()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[0xF] = registers[x] >= registers[y];
    registers[x] -= registers[y];
}

void Processor::OP_8xy6()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = registers[x] & 0x1u;
    registers[x] >>= 1;
}

void Processor::OP_8xy7()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    registers[0xF] = registers[y] >= registers[x];
    registers[x] = registers[y] - registers[x];
}

void Processor::OP_8xyE()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    registers[0xF] = (registers[x] & 0x80u) >> 7u;
    registers[x] <<= 1;
}

void Processor::OP_9xy0()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    if (registers[x] != registers[y])
        pc += 2;
}

void Processor::OP_Annn()
{
    index = opcode & 0x0FFFu;
}

void Processor::OP_Bnnn()
{
    pc = (opcode & 0x0FFFu) + registers[0];
}

void Processor::OP_Cxnn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;
    registers[x] = randGen() & byte;
}

void Processor::OP_Dxyn()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;
    uint8_t height = opcode & 0x000Fu;
    uint8_t x_pos = registers[x] % VIDEO_WIDTH;
    uint8_t y_pos = registers[y] % VIDEO_HEIGHT;

    registers[0xF] = 0;

    for (uint8_t row = 0; row < height; ++row)
    {
        if (index + row >= MEM_SIZE_BYTES)
            break;
        uint8_t sprite = memory[index + row];

        for (uint8_t col = 0; col < 8; ++col)
        {
            uint8_t px = x_pos + col;
            uint8_t py = y_pos + row;
            if (px >= VIDEO_WIDTH || py >= VIDEO_HEIGHT)
                continue;

            uint32_t& pixel = video[py * VIDEO_WIDTH + px];
            if (sprite & (0x80u >> col))
            {
                if (pixel)
                    registers[0xF] = 1;
                pixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Processor::OP_Ex9e()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    if (registers[x] < NUM_KEYS && keypad[registers[x]])
        pc += 2;
}

void Processor::OP_Exa1()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    if (registers[x] < NUM_KEYS && !keypad[registers[x]])
        pc += 2;
}

void Processor::OP_Fx07()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    registers[x] = delay_timer;
}

void Processor::OP_Fx0a()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    bool pressed = false;

    for (int i = 0; i < NUM_KEYS; ++i)
    {
        if (keypad[i])
        {
            registers[x] = i;
            pressed = true;
            break;
        }
    }

    if (!pressed)
        pc -= 2;
}

void Processor::OP_Fx15()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    delay_timer = registers[x];
}

void Processor::OP_Fx18()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    sound_timer = registers[x];
}

void Processor::OP_Fx1e()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    if (index + registers[x] < MEM_SIZE_BYTES)
        index += registers[x];
}

void Processor::OP_Fx29()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = registers[x];
    if (digit < 16)
        index = FONTSET_START_ADDRESS + (5 * digit);
}

void Processor::OP_Fx33()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t value = registers[x];
    memory[index] = value / 100;
    memory[index + 1] = (value / 10) % 10;
    memory[index + 2] = value % 10;
}

void Processor::OP_Fx55()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i <= x; ++i)
    {
        if (index + i < MEM_SIZE_BYTES)
            memory[index + i] = registers[i];
    }
}

void Processor::OP_Fx65()
{
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    for (uint8_t i = 0; i <= x; ++i)
    {
        if (index + i < MEM_SIZE_BYTES)
            registers[i] = memory[index + i];
    }
}

void Processor::OP_NULL()
{
    // Do nothing
}
