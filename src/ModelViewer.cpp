#include <iostream>

#include "Renderer.h"

int main()
{
    Renderer renderer;

    renderer.Run();

    std::cout << "Stopped!" << std::endl;

    return 0;
}