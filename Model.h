#pragma once

#include <memory>

#include "OGLUtils.hpp"

#include "VAO.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

struct VertexData
{
	float x, y, z;
	float r, g, b;
	float u, v;
	float nx, ny, nz;
};

class Model
{
public:

	Model();
	~Model();

	bool CreateFromOBJ(const char* filename);
	bool LoadTexture(const char* filename);

	void UpdatePosition(const glm::vec3 position);
	void UpdatePositionRotation(const glm::vec3 position, const float angle);

	inline uint32_t GetElementsCount() const
	{
		if (!ebo)
			return 0;

		return this->ebo->GetElementsCount();
	}

	void Draw(const bool &useTexture) const;
	void SetModelMatrix(const int &shader) const;
	
	void Delete();

private:
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ebo;
	std::unique_ptr<VAO> vao;
	std::unique_ptr<Texture> texture;

	glm::vec3 position;
	glm::mat4 model;
};

