#include "Camera.h"
#include "OGLUtils.hpp"

Camera::Camera(uint32_t width, uint32_t height, glm::vec3 position)
{
	this->width = width;
	this->height = height;
	this->position = position;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->orientation = glm::vec3(0.0f, 0.0f, 1.0f);
}

void Camera::CheckInputs(GLFWwindow* window)
{
	if (!window)
		return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += orientation * 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position += -orientation * 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		orientation = glm::rotate(orientation, glm::radians(1.0f), up);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		orientation = glm::rotate(orientation, glm::radians(-1.0f), up);
	}
}

void Camera::ApplyCamMatrix(float fov, float near, float far, int shader, const char* uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	view = glm::lookAt(position, position + orientation, up);
	proj = glm::perspective(glm::radians(fov), (float)(width / height), near, far);

	GLCall(glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(proj * view)));
}