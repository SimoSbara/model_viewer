#include <iostream>
#include "Windows.h"

#include "Renderer.h"

int main()
{
    Renderer renderer;
    renderer.Start();

    while (renderer.IsRunning())
    {
        Sleep(100);
    }

    std::cout << "Stopped!" << std::endl;

    return 0;
}