#pragma once

#include <string>

class ShaderManager
{
public:
	static bool CompileShaderFromFile(const int &program, const std::string &filename, uint32_t type);
	static bool CompileShaderFromSource(const int& program, const std::string& source, uint32_t type);

	static int CreateProgram();
	static void LinkValidateProgram(const int& program);
	static void UseProgram(const int program);
	static void DeleteProgram(int& program);
};

