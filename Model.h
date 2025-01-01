#pragma once

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VAO.h"

struct Indices
{
	uint32_t i1, i2, i3;
};

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
	bool CreateFromData(const float* vertexBuffer, const uint32_t vertexBufferSize, const uint32_t* indices, const uint32_t indicesSize);

	uint32_t GetElementsCount() const;

	void Bind() const;
	void UnBind() const;
	void Delete();

private:
	std::unique_ptr<VertexBuffer> vbo;
	std::unique_ptr<IndexBuffer> ebo;
	std::unique_ptr<VAO> vao;
};

