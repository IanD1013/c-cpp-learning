// Application.hpp
#pragma once

#include <SDL3/SDL.h>

struct Application
{
    public:
        Application(int argc, char* argv[])
        {
            StartUp();
        }

        ~Application()
        {
            ShutDown();
        }

        void StartUp()
        {
            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            }

            // Create our window
            mWindow = SDL_CreateWindow("An SDL3 Window", 640, 480, 0);

            // Empty pixels for now -- nothing drawn to our window
            SDL_Surface* windowSurface = SDL_GetWindowSurface(mWindow);

            // Load an art asset
            SDL_Surface* mySurface = SDL_LoadBMP("./assets/space_invader.bmp");
            if (mySurface == NULL)
            {
                SDL_Log("Could not find image");
            }

            // Copy that art asset, to the window
            SDL_BlitSurface(mySurface, NULL, windowSurface, NULL);
            SDL_UpdateWindowSurface(mWindow);

            // Free our 'mySurface', because we've copied the pixels to our window
            SDL_DestroySurface(mySurface);
        }

        void ShutDown()
        {
            SDL_DestroyWindow(mWindow);
            SDL_Quit();
        }

        void Input()
        {
            SDL_Event event;
            // Processing input
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    SDL_Log("Program quit %" SDL_PRIu64, event.quit.timestamp);
                    mRunning = false;
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

        void Update()
        {
        }

        void Render()
        {
        }

        void Loop()
        {
            // Our infinite game/application loop
            while (mRunning)
            {
                Input();
                Update();
                Render();
            }
        }

    private:
        bool        mRunning { true };
        SDL_Window* mWindow;
};
