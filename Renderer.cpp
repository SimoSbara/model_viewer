#include "Renderer.h"
#include "OGLUtils.hpp"
#include "ShaderManager.h"

#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


//QUADRATO
//XYZ RGB
//static float positions[] =
//{
//    -0.5f,  0.5f, 0.5f, 0.f, 1.f, 0.f, //0
//    -0.5f, -0.5f, 0.5f, 1.f, 0.f, 0.f, //1
//     0.5f, -0.5f, 0.5f, 0.f, 0.f, 1.f, //2
//     0.5f,  0.5f, 0.5f, 1.f, 1.f, 0.f  //3
//};


// 0--------3
// |        |
// |        |
// 1--------2

//static uint32_t indeces[] =
//{
//    0, 1, 3,
//    1, 2, 3
//};

//CUBO
//XYZ RGB UV 8 vertici
static float vertices[] =
{
     0.5f,  0.0f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 0.0f, //0
     0.0f,  0.0f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 0.0f, //1
     0.0f,  0.0f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 1.0f, //2
     0.5f,  0.0f, 0.5f, 1.f, 1.f, 0.f, 1.0f, 1.0f, //3

     0.5f,  0.5f, 0.0f, 0.f, 1.f, 0.f, 1.0f, 1.0f, //4
     0.0f,  0.5f, 0.0f, 1.f, 0.f, 0.f, 0.0f, 1.0f, //5
     0.0f,  0.5f, 0.5f, 0.f, 0.f, 1.f, 0.0f, 1.0f, //6
     0.5f,  0.5f, 0.5f, 1.f, 1.f, 0.f, 1.0f, 1.0f  //7
};


// 0--------3
// |        |
// |        |
// 1--------2

static uint32_t indeces[] =
{
    //BOTTOM
    0, 3, 2,
    0, 2, 1,
    //NORD
    2, 3, 7,
    2, 7, 6,
    //OVEST
    1, 2, 6,
    1, 6, 5,
    //EST
    0, 3, 7,
    0, 7, 4,
    //SUD
    1, 0, 4,
    1, 4, 5,
    //TOP
    5, 4, 6,
    4, 7, 6
};

Renderer* Renderer::renderer = nullptr;

Renderer::Renderer()
{
    this->width = 1000;
    this->height = 800;
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
    model.CreateFromOBJ("models/perno_samp3.obj");
    model.LoadTexture("samp.png");

    return true;
}

bool Renderer::FreeResources()
{
    model.Delete();

    ShaderManager::DeleteProgram(shaderTexture);
    ShaderManager::DeleteProgram(shaderColor);

    return true;
}

void Renderer::CheckInputs(Camera& camera, bool& useTexture, float& scale, float& rotation)
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
        scale = fmax(0.1f, scale - 0.01f);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        scale = fmin(2.0f, scale + 0.01f);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotation -= 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotation += 1.0f;
    }

    camera.CheckInputs(window);
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
    int& shaderTexture = renderer.shaderTexture;
    int& shaderColor = renderer.shaderColor;

    Model& mdl = renderer.model;

    Camera camera(renderer.width, renderer.height, glm::vec3(0.0f, 0.0f, -5.0f));

    int shader;
    bool alternate = false;
    bool colorAlternate = false;
    float scale = 1.0f;
    float rotation = 0.0f;
    float red = 0.0f;
    bool useTexture = true;

    bool isTPressed = false;

    double lastTime = glfwGetTime();
    int frames = 0;
    
    if (!window)
    {
        renderer.isRunning = false;
        return;
    }

    GLCall(glEnable(GL_DEPTH_TEST));

    while (renderer.isRunning && !glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double diff = currentTime - lastTime;
        
        if (diff >= 0.2f)
        {
            char stats[64];

            double frameTime = (diff * 1000.0) / double(frames);
            
            sprintf_s(stats, 64, "Model Viewer %.2f fps - %.2f ms/frame", (double)frames / diff, frameTime);

            system("cls");
            printf("%f fps\n", (double)frames / diff);
            printf("%f ms/frame\n", frameTime);
            printf("useTexture %d\n", useTexture);
            printf("position (%.2f %.2f %.2f)\n", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

            glfwSetWindowTitle(window, stats);
            frames = 0;
            lastTime += diff;
        }

        frames++;

        renderer.CheckInputs(camera, useTexture, scale, rotation);

        shader = (useTexture) ? shaderTexture : shaderColor;
        ShaderManager::UseProgram(shader);

        camera.ApplyCamMatrix(45, 0.1f, 100.0f, shader, "camMatrix");
        glUniform1f(glGetUniformLocation(shader, "scale"), scale);
        glUniform3f(glGetUniformLocation(shader, "lightDir"), camera.GetOrientation().x, camera.GetOrientation().y, camera.GetOrientation().z);

        //clear screen
        GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        mdl.UpdatePositionRotation(glm::vec3(0.0f, 0.0f, 0.0f), rotation);
        mdl.SetModelMatrix(shader);
        mdl.Draw(useTexture);

        ShaderManager::UseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    renderer.FreeResources();
    renderer.FreeOpenGL();

    renderer.isRunning = false;
}