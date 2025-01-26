#pragma once

#include <thread>
#include <memory>
#include <unordered_map>

#include "Assets/Model.h"
#include "Assets/Camera.h"

class GLFWwindow;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void SetWindowDimensions(uint16_t width, uint16_t height);

    bool Run();

private:
    bool InitOpenGL();
    bool FreeOpenGL();
    bool InitResources();
    bool FreeResources();

    void CheckInputs(Camera& camera, bool& useTexture, float& scale, float& rX, float& rY);

private:
    uint16_t width, height;

    //risorse opengl
    GLFWwindow* window;
    int shaderTexture;
    int shaderColor;

    Model model;
    Model light;

    std::unordered_map<int, bool> keys;
};

