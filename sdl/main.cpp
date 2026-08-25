#include <iostream>
#include <SDL3/SDL.h>

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create our window
    SDL_Window *window = SDL_CreateWindow("An SDL3 Window", 640, 480, SDL_WINDOW_OPENGL);

    SDL_Delay(10000);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}