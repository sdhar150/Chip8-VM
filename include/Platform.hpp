/******************************************************************************
 * CHIP-8 Emulator
 * Author: Soham Dhar
 * Date: 2025-10-27
 *
 * Description: Describes the required display functions
 *****************************************************************************/
#include <SDL2/SDL.h>
#include <cstdint>

class Platform
{
  public:
    Platform(const char* title, int windowWidth, int windowHeight,
             int textureWidth, int textureHeight);
    ~Platform();

    void Update(const void* buffer, int pitch);
    bool ProcessInput(uint8_t* keys);

  private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    int windowWidth{};
    int windowHeight{};
    int textureWidth{};
    int textureHeight{};
};
