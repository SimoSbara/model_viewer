#include <iostream>
#include "Windows.h"

#include "Renderer.h"

int main()
{
    Renderer::Get()->Start();

    while (Renderer::Get()->IsRunning())
    {
        Sleep(100);
    }

    std::cout << "Stopped!" << std::endl;

    return 0;
}