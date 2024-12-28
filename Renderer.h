#pragma once

#include <thread>
#include <memory>

#include "VAO.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class GLFWwindow;

class Renderer
{
public:

    static Renderer* Get()
    {
        if (!renderer)
            renderer = new Renderer;

        return renderer;
    }

    void SetWindowDimensions(uint16_t width, uint16_t height);

    void Start();
    void Stop();

    inline bool IsRunning() const
    {
        return this->isRunning;
    }

    static void RenderThread(Renderer* r);

private:
    Renderer();

    bool InitOpenGL();
    bool FreeOpenGL();
    bool InitResources();
    bool FreeResources();

private:
    static Renderer* renderer;

    uint16_t width, height;
    bool isRunning;

    std::thread renderThread;

    //risorse opengl
    GLFWwindow* window;
    int shaderProgram;
    int scaleUniformLoc;

    std::unique_ptr<VertexBuffer> vbo;
    std::unique_ptr<IndexBuffer> ebo;
    std::unique_ptr<VAO> vao;
};

