/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Implements the display and input collection
 *****************************************************************************/

#include "Platform.hpp"
#include <iostream>
#include <stdexcept>

Platform::Platform(const char* title, int windowWidth, int windowHeight,
                   int textureWidth, int textureHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      textureWidth(textureWidth), textureHeight(textureHeight)
{
    if (windowWidth <= 0 || windowHeight <= 0)
    {
        throw std::invalid_argument("Invalid window dimensions");
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());
    }

    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              windowWidth, windowHeight,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::string msg = "SDL_CreateWindow failed: ";
        msg += SDL_GetError();
        SDL_Quit();
        throw std::runtime_error(msg);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::string msg = "SDL_CreateRenderer failed: ";
        msg += SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(msg);
    }

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STREAMING,
                                textureWidth, textureHeight);
    if (!texture)
    {
        std::string msg = "SDL_CreateTexture failed: ";
        msg += SDL_GetError();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error(msg);
    }
}

Platform::~Platform()
{
    if (texture)
        SDL_DestroyTexture(texture);
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void Platform::Update(const void* buffer, int pitch)
{
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Platform::ProcessInput(uint8_t* keys)
{
    bool quit = false;
    SDL_Event event;

    static constexpr struct
    {
        SDL_Keycode key;
        uint8_t index;
    } keyMap[] =
        {
            {SDLK_x, 0x0}, {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_z, 0xA}, {SDLK_c, 0xB}, {SDLK_4, 0xC}, {SDLK_r, 0xD}, {SDLK_f, 0xE}, {SDLK_v, 0xF}};

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            bool pressed = (event.type == SDL_KEYDOWN);

            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
                break;
            }

            for (const auto& m : keyMap)
            {
                if (event.key.keysym.sym == m.key)
                {
                    keys[m.index] = pressed ? 1 : 0;
                    break;
                }
            }
            break;
        }
        }
    }

    return quit;
}
