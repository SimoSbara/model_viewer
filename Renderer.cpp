#include "Renderer.h"
#include "OGLUtils.hpp"
#include "ShaderManager.h"

#include <GLFW/glfw3.h>

//XYZ RGB
static float positions[] =
{
    -0.5f,  0.5f, 1.f, 0.f, 1.f, 0.f, //0
    -0.5f, -0.5f, 1.f, 1.f, 0.f, 0.f, //1
     0.5f, -0.5f, 1.f, 0.f, 0.f, 1.f, //2
     0.5f,  0.5f, 1.f, 1.f, 1.f, 0.f  //3
};


// 0--------3
// |        |
// |        |
// 1--------2

static uint32_t indeces[] =
{
    0, 1, 3,
    1, 2, 3
};

Renderer* Renderer::renderer = nullptr;

Renderer::Renderer()
{
    this->width = 640;
    this->height = 480;
    this->shaderProgram = 0;
    this->scaleUniformLoc = 0;
    this->window = nullptr;
    this->isRunning = false;
}

void Renderer::SetWindowDimensions(uint16_t width, uint16_t height)
{
    if (IsRunning())
        return;

    this->width = width;
    this->height = height;
}

void Renderer::Start()
{
    renderThread = std::thread(RenderThread, this);
    isRunning = true;
}

void Renderer::Stop()
{
    isRunning = false;

    renderThread.join();
}

bool Renderer::InitOpenGL()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Model Viewer", NULL, NULL);
    if (!window)
    {
        std::cout << "Error GLFW";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //vsync

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error GLEW";
        return false;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, width, height);

    return true;
}

bool Renderer::FreeOpenGL()
{
    glfwTerminate();

    return true;
}

bool Renderer::InitResources()
{
    uint32_t vs, ps;

    shaderProgram = ShaderManager::CreateProgram();

    if (!ShaderManager::CompileShaderFromFile(shaderProgram, vs, "shaders/VertexShader.glsl", GL_VERTEX_SHADER))
        return false;

    if (!ShaderManager::CompileShaderFromFile(shaderProgram, ps, "shaders/PixelShader.glsl", GL_FRAGMENT_SHADER))
        return false;

    ShaderManager::LinkValidateProgram(shaderProgram);
    ShaderManager::DeleteShader(vs);
    ShaderManager::DeleteShader(ps);

    ShaderManager::UseProgram(shaderProgram);

    vbo.reset(new VertexBuffer(positions, sizeof(positions)));
    ebo.reset(new IndexBuffer(indeces, 6));
    vao.reset(new VAO);

    vao->Bind();

    vao->LinkVertexBuffer(*vbo, 0, GL_FLOAT, 3, 6 * sizeof(float), 0);
    vao->LinkVertexBuffer(*vbo, 1, GL_FLOAT, 3, 6 * sizeof(float), 3 * sizeof(float));

    vao->UnBind();
    vbo->UnBind();
    ebo->UnBind();

    scaleUniformLoc = glGetUniformLocation(shaderProgram, "scale");

    GLCall(glUniform1f(scaleUniformLoc, 1.0f));

    ShaderManager::UseProgram(0);

    return true;
}

bool Renderer::FreeResources()
{
    vbo.reset();
    ebo.reset();
    vao.reset();

    ShaderManager::DeleteProgram(shaderProgram);

    shaderProgram = 0;

    return true;
}

void Renderer::RenderThread(Renderer* r)
{
    if (!r)
        return;

    Renderer& renderer = *r;

    if (!renderer.InitOpenGL() || !renderer.InitResources())
    {
        renderer.isRunning = false;
        return;
    }

    GLFWwindow* window = renderer.window;
    int& shader = renderer.shaderProgram;
    int& location = renderer.scaleUniformLoc;

    VAO* vao = renderer.vao.get();
    VertexBuffer* vbo = renderer.vbo.get();
    IndexBuffer* ebo = renderer.ebo.get();

    bool alternate = false;
    float scale = 1.0f;

    if (!vao || !ebo || !window)
    {
        renderer.isRunning = false;
        return;
    }

    while (renderer.isRunning && !glfwWindowShouldClose(window))
    {
        //clear screen
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ShaderManager::UseProgram(shader);

        vao->Bind();
        ebo->Bind();

        if (alternate)
            scale += 0.001f;
        else
            scale -= 0.001f;

        if (scale >= 1.f)
            alternate = false;
        else if (scale <= 0.1f)
            alternate = true;

        glUniform1f(location, scale);

        glDrawElements(GL_TRIANGLES, sizeof(indeces) / sizeof(uint32_t), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    renderer.FreeResources();
    renderer.FreeOpenGL();

    renderer.isRunning = false;
}