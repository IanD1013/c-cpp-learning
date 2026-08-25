#include <iostream>
#include <SDL3/SDL.h>

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }
    else
    {
        std::cout << "SDL is running!!!\n";
    }

    SDL_Quit();

    return 0;
}