#pragma once

#include <iostream>
#include <GL/glew.h>

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    bool isClear = true;

    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") " << function <<
            " " << file << ":" << line << std::endl;

        isClear = false;
    }

    return isClear;
}