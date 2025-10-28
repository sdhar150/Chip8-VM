/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Main file
 *****************************************************************************/

#include "Platform.hpp"
#include "chip8.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <stdexcept>

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay(ms)> <ROM>\n";
        return EXIT_FAILURE;
    }

    try
    {
        int videoScale = std::stoi(argv[1]);
        int cycleDelay = std::stoi(argv[2]);
        const std::string romFile = argv[3];

        constexpr int VIDEO_WIDTH = 64;
        constexpr int VIDEO_HEIGHT = 32;

        const int windowWidth = VIDEO_WIDTH * videoScale;
        const int windowHeight = VIDEO_HEIGHT * videoScale;

        Platform platform("CHIP-8 Emulator", windowWidth, windowHeight, VIDEO_WIDTH, VIDEO_HEIGHT);
        Processor chip8;

        if (chip8.load_rom(const_cast<char *>(romFile.c_str())) != 0)
        {
            std::cerr << "Failed to load ROM: " << romFile << "\n";
            return EXIT_FAILURE;
        }

        const int videoPitch = static_cast<int>(sizeof(chip8.video[0]) * VIDEO_WIDTH);
        bool quit = false;

        auto lastCycleTime = std::chrono::high_resolution_clock::now();

        while (!quit)
        {
            quit = platform.ProcessInput(chip8.keypad);

            auto currentTime = std::chrono::high_resolution_clock::now();
            auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastCycleTime).count();

            if (delta >= cycleDelay)
            {
                lastCycleTime = currentTime;
                chip8.cycle();
                platform.Update(chip8.video, videoPitch);
            }

            SDL_Delay(1);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
