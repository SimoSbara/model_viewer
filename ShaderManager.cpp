#include "ShaderManager.h"
#include "OGLUtils.hpp"

#include <fstream>

int ShaderManager::CreateProgram()
{
	return glCreateProgram();
}

bool ShaderManager::CompileShaderFromSource(const int& program, uint32_t& shaderID, const std::string& source, uint32_t type)
{
    int status;
    shaderID = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(shaderID, 1, &src, NULL));
    GLCall(glCompileShader(shaderID));

    GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));

    if (status != GL_TRUE)
    {
        int length;

        GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));

        char* message = new char[length];

        GLCall(glGetShaderInfoLog(shaderID, length, &length, message));

        std::cout << "error compile shader " << message << std::endl;
        std::cout << "shader " << source << std::endl;

        delete[] message;

        return false;
    }

    GLCall(glAttachShader(program, shaderID));

    return true;
}

bool ShaderManager::CompileShaderFromFile(const int& program, uint32_t& shaderID, const std::string& filename, uint32_t type)
{
	char buf[512];

    std::string sourceCode;
    std::ifstream shaderStream(filename, std::ios::in);

    if (!shaderStream.is_open())
        return false;

    while (shaderStream.getline(buf, 512))
    {
        sourceCode += buf;
        sourceCode += "\n";
    }

    std::cout << sourceCode << std::endl;

    shaderStream.close();

    return CompileShaderFromSource(program, shaderID, sourceCode, type);
}

void ShaderManager::DeleteShader(const int& shaderID)
{
    GLCall(glDeleteShader(shaderID));
}

void ShaderManager::LinkValidateProgram(const int& program)
{
    GLCall(glLinkProgram(program));
    //GLCall(glValidateProgram(program));
}

void ShaderManager::UseProgram(const int program)
{
    GLCall(glUseProgram(program));
}

void ShaderManager::DeleteProgram(const int& program)
{
    GLCall(glDeleteProgram(program));
}