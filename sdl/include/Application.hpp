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
