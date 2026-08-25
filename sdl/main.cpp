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

    // Our infinite game/application loop
    bool running = true;
    while (running)
    {
        SDL_Event event;
        // Processing input
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                SDL_Log("Program quit %" SDL_PRIu64, event.quit.timestamp);
                running = false;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                SDL_Log("Some key was pressed down");
                if (event.key.key == SDLK_0)
                {
                    SDL_Log("0 was pressed");
                }
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}