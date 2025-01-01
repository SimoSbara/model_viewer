#include "Renderer.h"
#include "OGLUtils.hpp"
#include "ShaderManager.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    texture.CreateFromFile("samp.png");
    model.CreateFromOBJ("models/perno_samp.obj");

    return true;
}

bool Renderer::FreeResources()
{
    model.Delete();
    texture.Delete();

    ShaderManager::DeleteProgram(shaderTexture);
    ShaderManager::DeleteProgram(shaderColor);

    return true;
}

void Renderer::Projection(const float rotation, const int& shader)
{
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 proj(1.0f);

    //model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.5f, 0.2f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));
    proj = glm::perspective(glm::radians(45.0f),
        (float)width / (float)height,
        0.1f, 100.f);

    int modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(shader, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
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
    Texture& texture = renderer.texture;

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
        
        if (diff >= 1.0f)
        {
            double frameTime = (diff * 1000.0) / double(frames);

            system("cls");
            printf("%f fps\n", (double)frames / diff);
            printf("%f ms/frame\n", frameTime);
            printf("useTexture %d\n", useTexture);
            frames = 0;
            lastTime += diff;
        }

        frames++;

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE && isTPressed)
        {
            isTPressed = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !isTPressed)
        {
            printf("glfwGetKey T\n");
            useTexture = !useTexture;

            isTPressed = true;
        }

        shader = (useTexture) ? shaderTexture : shaderColor;
        ShaderManager::UseProgram(shader);

        //clear screen
        GLCall(glClearColor(0.07f, 0.13f, 0.17f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        
        renderer.Projection(rotation, shader);

        if (alternate)
            scale += 0.005f;
        else
            scale -= 0.005f;

        if (scale >= 1.f)
            alternate = false;
        else if (scale <= 0.05f)
            alternate = true;

        rotation += 0.5f;

        //glUniform1f(glGetUniformLocation(shader, "scale"), scale);

        mdl.Bind();

        if(useTexture)
            texture.Bind();
        else
        {

            if (colorAlternate)
                red += 0.015f;
            else
                red -= 0.015f;

            if (red >= 1.f)
                colorAlternate = false;
            else if (red <= 0.f)
                colorAlternate = true;

            float r = (float)(rand() % 256) / 255.0f;
            float g = (float)(rand() % 256) / 255.0f;
            float b = (float)(rand() % 256) / 255.0f;

            GLCall(glUniform3f(glGetUniformLocation(shader, "color"), 1.0f, 0.0f, 0.0f));
        }

        glDrawElements(GL_TRIANGLES, mdl.GetElementsCount(), GL_UNSIGNED_INT, 0);

        if (useTexture)
            texture.UnBind();

        mdl.UnBind();

        ShaderManager::UseProgram(0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    renderer.FreeResources();
    renderer.FreeOpenGL();

    renderer.isRunning = false;
}