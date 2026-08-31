// Application.hpp
#pragma once

#include <SDL3/SDL.h>
#include <cstdlib>

struct MovingRectangle
{
    public:
        MovingRectangle()
        {
            // Randomize the speed
            mSpeed *= std::rand() % 10 / 10.0f;
            // Randomize position
            mRectangle.x = std::rand() % 640;
            mRectangle.y = std::rand() % 480;
            // SDL_Log("%f, %f", mRectangle.x, mRectangle.y);
            // Adding '10' so they are not '0' in size
            mRectangle.w = std::rand() % 50 + 10;
            mRectangle.h = std::rand() % 50 + 10;
            // true or false value
            xPositiveDirection = std::rand() % 1;
            yPositiveDirection = std::rand() % 1;
        }

        void Update()
        {
            if (mRectangle.x > 640.0f - mRectangle.w)
            {
                xPositiveDirection = false;
            }
            if (mRectangle.x < 0.0f)
            {
                xPositiveDirection = true;
            }
            if (mRectangle.y > 480.0f - mRectangle.h)
            {
                yPositiveDirection = false;
            }
            if (mRectangle.y < 0.0f)
            {
                yPositiveDirection = true;
            }

            if (xPositiveDirection)
            {
                mRectangle.x += mSpeed;
            }
            else
            {
                mRectangle.x -= mSpeed;
            }
            if (yPositiveDirection)
            {
                mRectangle.y += mSpeed;
            }
            else
            {
                mRectangle.y -= mSpeed;
            }
        }

        void Render(SDL_Renderer* renderer)
        {
            SDL_RenderRect(renderer, &mRectangle);
        }

    private:
        SDL_FRect mRectangle { 20.0f, 20.0f, 50.0f, 60.f };
        bool      xPositiveDirection;
        bool      yPositiveDirection;
        float     mSpeed { 1.0f };
};

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
            for (int i = 0; i < 30; i++)
            {
                mRectangles[i].Update();
            }
        }

        void Render()
        {
            SDL_SetRenderDrawColor(mRenderer, 0, 64, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(mRenderer);

            SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            for (int i = 0; i < 30; i++)
            {
                mRectangles[i].Render(mRenderer);
            }

            SDL_RenderPresent(mRenderer);
        }

        void Loop()
        {
            // Our infinite game/application loop
            Uint64 lastTime, currentTime;

            // Record our 'starting time'
            lastTime             = SDL_GetTicks();
            Uint64 framesElapsed = 0;

            while (mRunning)
            {
                Uint64 startOfFrame = SDL_GetTicks();
                // We want, input/update/render to take 16ms
                Input();
                Update();
                Render();
                Uint64 elapsedTime = SDL_GetTicks() - startOfFrame;

                framesElapsed++;

                // Time keeping code - for frames elapsed
                currentTime = SDL_GetTicks();

                // Insert a 'frame cap' so that our program does not run too fast
                if (elapsedTime < 1000 / 60)
                {
                    Uint64 delay = 1000 / 60 - elapsedTime;
                    SDL_Delay(delay);
                    // SDL_Log("elapsedTime: %li", elapsedTime);
                    // SDL_Log("delaying by: %li", delay);
                }

                // If 1s passes, report how many frames have been executed
                if (currentTime > lastTime + 1000)
                {
                    SDL_Log("1 second has elapsed");
                    SDL_Log("%li", framesElapsed);
                    framesElapsed = 0;
                    lastTime      = SDL_GetTicks();
                }
            }
        }

    private:
        bool            mRunning { true };
        SDL_Window*     mWindow;
        SDL_Renderer*   mRenderer;
        MovingRectangle mRectangles[30];
};
