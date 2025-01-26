#include "Renderer.h"
#include "OGLUtils.hpp"
#include "Managers/ShaderManager.h"

#include <sstream>
#include <iomanip> 

//QUADRATO
//XYZ RGB UV NORMALS
//static float positions[] =
//{
//    -0.5f,  0.5f, 0.5f, 0.f, 1.f, 0.f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, //0
//    -0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //1
//     0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, //2
//     0.5f,  0.5f, 0.5f, 1.f, 1.f, 0.f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f  //3
//};

static float positions[] =
{
    -1.0f,  0.0f, 1.0f, 0.f, 1.f, 0.f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //0
     1.0f,  0.0f, 1.0f, 1.f, 0.f, 0.f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, //1
    -1.0f,  0.0f, -1.0f, 0.f, 0.f, 1.f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, //2
     1.0f,  0.0f, -1.0f, 1.f, 1.f, 0.f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  //3
};



// 0--------3
// |        |
// |        |
// 1--------2

//static uint32_t indeces[] =
//{
//    0, 1, 3,
//    1, 2, 3
//};

static uint32_t indeces[] =
{
    1, 2, 0,
    1, 3, 2
};

//CUBO
//XYZ RGB UV 8 vertici
//static float vertices[] =
//{
//     0.5f,  0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f, //0
//     0.0f,  0.0f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 0.0f, //1
//     0.0f,  0.0f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 1.0f, //2
//     0.5f,  0.0f, 0.5f, 1.f, 1.f, 0.f, 1.0f, 1.0f, //3
//
//     0.5f,  0.5f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f, //4
//     0.0f,  0.5f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 1.0f, //5
//     0.0f,  0.5f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 1.0f, //6
//     0.5f,  0.5f, 0.5f, 1.f, 1.f, 0.f, 1.0f, 1.0f  //7
//};


// 0--------3
// |        |
// |        |
// 1--------2

//static uint32_t indeces[] =
//{
//    //BOTTOM
//    0, 3, 2,
//    0, 2, 1,
//    //NORD
//    2, 3, 7,
//    2, 7, 6,
//    //OVEST
//    1, 2, 6,
//    1, 6, 5,
//    //EST
//    0, 3, 7,
//    0, 7, 4,
//    //SUD
//    1, 0, 4,
//    1, 4, 5,
//    //TOP
//    5, 4, 6,
//    4, 7, 6
//};

Renderer::Renderer()
{
    this->width = 1000;
    this->height = 800;
    this->window = nullptr;
}

Renderer::~Renderer()
{

}

void Renderer::SetWindowDimensions(uint16_t width, uint16_t height)
{
    this->width = width;
    this->height = height;
}

bool Renderer::InitOpenGL()
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //uncomment this statement to fix compilation on OS X
#endif

    window = glfwCreateWindow(width, height, "Model Viewer", NULL, NULL);
    if (!window)
    {
        std::cout << "Error GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //vsync

#ifndef __APPLE__
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error GLEW";
        return false;
    }
#else
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//if (gladLoadGL())
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
#endif

    std::cout << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, width, height);

    GLCall(glEnable(GL_DEPTH_TEST));

    return true;
}

bool Renderer::FreeOpenGL()
{
    glfwTerminate();

    return true;
}

bool Renderer::InitResources()
{
    shaderTexture = ShaderManager::CreateProgram();
    shaderColor = ShaderManager::CreateProgram();

    if (!ShaderManager::CompileShaderFromFile(shaderTexture, "shaders/texture/VertexShader.glsl", GL_VERTEX_SHADER))
        return false;

    if (!ShaderManager::CompileShaderFromFile(shaderTexture, "shaders/texture/PixelShader.glsl", GL_FRAGMENT_SHADER))
        return false;

    ShaderManager::LinkValidateProgram(shaderTexture);
    ShaderManager::UseProgram(0);

    if (!ShaderManager::CompileShaderFromFile(shaderColor, "shaders/color/VertexShader.glsl", GL_VERTEX_SHADER))
        return false;

    if (!ShaderManager::CompileShaderFromFile(shaderColor, "shaders/color/PixelShader.glsl", GL_FRAGMENT_SHADER))
        return false;

    ShaderManager::LinkValidateProgram(shaderColor);
    ShaderManager::UseProgram(0);

    //setto le location delle variabili per lo shader per rendering di texture
    ShaderManager::UseProgram(shaderTexture);

    GLCall(glUniform1f(glGetUniformLocation(shaderTexture, "scale"), 1.0f));
    GLCall(glUniform1i(glGetUniformLocation(shaderTexture, "tex0"), 0));

    ShaderManager::UseProgram(0);

    //setto le location delle variabili per lo shader per rendering colorato
    ShaderManager::UseProgram(shaderColor);

    GLCall(glUniform1f(glGetUniformLocation(shaderColor, "scale"), 1.0f));
    GLCall(glUniform3f(glGetUniformLocation(shaderColor, "color"), 1.0f, 1.0f, 0.0f));

    ShaderManager::UseProgram(0);

    //carico assets
    model.CreateFromOBJ("models/alfa147.obj");
    model.LoadTexture("models/samp.png");

    return true;
}

bool Renderer::FreeResources()
{
    model.Delete();

    ShaderManager::DeleteProgram(shaderTexture);
    ShaderManager::DeleteProgram(shaderColor);

    return true;
}

void Renderer::CheckInputs(Camera& camera, bool& useTexture, float& scale, float& rX, float& rY)
{
    if (!window)
        return;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && keys[GLFW_KEY_T])
    {
        keys[GLFW_KEY_T] = false;
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !keys[GLFW_KEY_T])
    {
        useTexture = !useTexture;
        keys[GLFW_KEY_T] = true;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        //scale = fmax(0.1f, scale - 0.01f);
        rX -= 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        //scale = fmin(2.0f, scale + 0.01f);
        rX += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rY -= 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rY += 1.0f;
    }

    camera.CheckInputs(window);
}

bool Renderer::Run()
{
    if (!InitOpenGL() || !InitResources())
    {
        std::cout << "Failed to initialize!" << std::endl;
        return false;
    }

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, -5.0f));

    int shader;
    bool alternate = false;
    bool colorAlternate = false;
    float scale = 1.0f;
    float rX = 0.0f;
    float rY = 0.0f;
    float red = 0.0f;
    bool useTexture = true;

    bool isTPressed = false;

    double lastTime = glfwGetTime();
    int frames = 0;

    GLCall(glEnable(GL_DEPTH_TEST));

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double diff = currentTime - lastTime;
        
        if (diff >= 0.2f)
        {
            std::ostringstream stats;

            double frameTime = (diff * 1000.0) / double(frames);
            double fps = (double)frames / diff;

            stats << "Model Viewer ";
            stats << std::fixed << std::setprecision(2) << fps << " fps - ";
            stats << std::fixed << std::setprecision(2) << frameTime << " ms/frame";

            glfwSetWindowTitle(window, stats.str().c_str());
            frames = 0;
            lastTime += diff;
        }

        frames++;

        CheckInputs(camera, useTexture, scale, rX, rY);

        shader = (useTexture) ? shaderTexture : shaderColor;
        ShaderManager::UseProgram(shader);

        camera.ApplyCamMatrix(45, 0.1f, 100.0f, shader, "camMatrix");
        glUniform1f(glGetUniformLocation(shader, "scale"), scale);
        glUniform3f(glGetUniformLocation(shader, "lightPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        //glUniform3f(glGetUniformLocation(shader, "lightDir"), camera.GetOrientation().x, camera.GetOrientation().y, camera.GetOrientation().z);

        //clear screen
        GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        model.UpdatePositionRotation(glm::vec3(0.0f, 0.0f, 0.0f), rX, rY);
        model.SetModelMatrix(shader);
        model.Draw(useTexture);

        ShaderManager::UseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    FreeResources();

    return true;
}