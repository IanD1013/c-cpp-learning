// g++ -std=c++20 ./src/main.cpp -I./include -o prog $(pkg-config --cflags --libs sdl3) && ./prog
#include "Application.hpp"

int main(int argc, char* argv[])
{
    Application app(argc, argv);
    app.Loop(120.0f);

    return 0;
}