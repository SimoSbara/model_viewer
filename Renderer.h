#pragma once

#include <thread>
#include <memory>
#include <unordered_map>

#include "Model.h"
#include "Camera.h"

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

    void CheckInputs(Camera& camera, bool& useTexture, float& scale, float& rX, float& rY);

private:
    static Renderer* renderer;

    uint16_t width, height;
    bool isRunning;

    std::thread renderThread;

    //risorse opengl
    GLFWwindow* window;
    int shaderTexture;
    int shaderColor;

    Model model;
    Model light;

    std::unordered_map<int, bool> keys;
};

