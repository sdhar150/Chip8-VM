/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Implements some basic CPU functions
 *****************************************************************************/

#include "chip8.hpp"
#include <random>
#include <fstream>
#include <cstring>
#include <iostream>

int Processor::load_rom(char *filename)
{
    std::ifstream rom(filename, std::ios::binary);

    if (!rom.is_open())
    {
        std::cerr << "Failed to open ROM: " << filename << "\n";
        return 1;
    }

    int available_memory = MEM_SIZE_BYTES - START_ADDRESS;
    rom.read(reinterpret_cast<char *>(memory + START_ADDRESS), available_memory);

    if (!rom && !rom.eof())
    {
        std::cerr << "Error reading ROM: " << filename << "\n";
        return 1;
    }

    return 0;
}

Processor::Processor()
{
    std::memset(video, 0, sizeof(video));
    pc = START_ADDRESS;

    for (size_t i = 0; i < FONTSET_SIZE; ++i)
    {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    table[0x0] = &Processor::Table0;
    table[0x1] = &Processor::OP_1nnn;
    table[0x2] = &Processor::OP_2nnn;
    table[0x3] = &Processor::OP_3xnn;
    table[0x4] = &Processor::OP_4xnn;
    table[0x5] = &Processor::OP_5xy0;
    table[0x6] = &Processor::OP_6xnn;
    table[0x7] = &Processor::OP_7xnn;
    table[0x8] = &Processor::Table8;
    table[0x9] = &Processor::OP_9xy0;
    table[0xA] = &Processor::OP_Annn;
    table[0xB] = &Processor::OP_Bnnn;
    table[0xC] = &Processor::OP_Cxnn;
    table[0xD] = &Processor::OP_Dxyn;
    table[0xE] = &Processor::TableE;
    table[0xF] = &Processor::TableF;

    for (size_t i = 0; i <= 0xE; ++i)
    {
        table0[i] = &Processor::OP_NULL;
        table8[i] = &Processor::OP_NULL;
        tableE[i] = &Processor::OP_NULL;
    }

    for (size_t i = 0; i <= 0x65; ++i)
    {
        tableF[i] = &Processor::OP_NULL;
    }

    table0[0x0] = &Processor::OP_00E0;
    table0[0xE] = &Processor::OP_00EE;

    table8[0x0] = &Processor::OP_8xy0;
    table8[0x1] = &Processor::OP_8xy1;
    table8[0x2] = &Processor::OP_8xy2;
    table8[0x3] = &Processor::OP_8xy3;
    table8[0x4] = &Processor::OP_8xy4;
    table8[0x5] = &Processor::OP_8xy5;
    table8[0x6] = &Processor::OP_8xy6;
    table8[0x7] = &Processor::OP_8xy7;
    table8[0xE] = &Processor::OP_8xyE;

    tableE[0x1] = &Processor::OP_Exa1;
    tableE[0xE] = &Processor::OP_Ex9e;

    tableF[0x07] = &Processor::OP_Fx07;
    tableF[0x0A] = &Processor::OP_Fx0a;
    tableF[0x15] = &Processor::OP_Fx15;
    tableF[0x18] = &Processor::OP_Fx18;
    tableF[0x1E] = &Processor::OP_Fx1e;
    tableF[0x29] = &Processor::OP_Fx29;
    tableF[0x33] = &Processor::OP_Fx33;
    tableF[0x55] = &Processor::OP_Fx55;
    tableF[0x65] = &Processor::OP_Fx65;
}

uint8_t Processor::randGen()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(0, 255);
    return static_cast<uint8_t>(distrib(gen));
}

void Processor::cycle()
{
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;

    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    if (delay_timer > 0)
    {
        --delay_timer;
    }

    if (sound_timer > 0)
    {
        --sound_timer;
    }
}
