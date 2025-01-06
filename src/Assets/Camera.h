#pragma once

#include "../OGLUtils.hpp"

#include <unordered_map>

class Camera
{
public:
	Camera(uint32_t width, uint32_t height, glm::vec3 position);

	void CheckInputs(GLFWwindow* window);
	void ApplyCamMatrix(float fov, float near, float far, int shader, const char* uniform);

	inline glm::vec3& GetPosition()
	{
		return this->position;
	}

	inline glm::vec3 GetOrientation() const
	{
		return this->orientation;
	}

private:
	glm::vec3 position, up, orientation;
	uint32_t width, height;
	std::unordered_map<int, bool> keys;
};

