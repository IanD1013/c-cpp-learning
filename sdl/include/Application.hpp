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

            mRenderer = SDL_CreateRenderer(mWindow, NULL);
            if (mRenderer == nullptr)
            {
                SDL_Log("Error creating renderer");
            }

            // Empty pixels for now -- nothing drawn to our window
            SDL_Surface* windowSurface = SDL_GetWindowSurface(mWindow);

            // Load an art asset
            SDL_Surface* mySurface = SDL_LoadBMP("./assets/space_invader.bmp");
            if (mySurface == nullptr)
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
            SDL_DestroyRenderer(mRenderer);
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
            SDL_SetRenderDrawColor(mRenderer, 0, 64, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(mRenderer);

            SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderLine(mRenderer, 0.0f, 0.0f, 640.0f, 480.0f);

            rectangle.x += 0.05f;
            if (rectangle.x > 640)
            {
                rectangle.x = 0.0f;
            }

            SDL_RenderRect(mRenderer, &rectangle);

            SDL_RenderPresent(mRenderer);
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
        bool          mRunning { true };
        SDL_Window*   mWindow;
        SDL_Renderer* mRenderer;
        SDL_FRect     rectangle { 20.0f, 20.0f, 50.0f, 60.0f };
};
