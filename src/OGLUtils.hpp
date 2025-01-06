#pragma once

#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

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