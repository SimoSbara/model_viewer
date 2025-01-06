#pragma once

#include <stdint.h>
#include "VertexBuffer.h"

class VAO
{
public:
	VAO();
	~VAO();

	void LinkVertexBuffer(VertexBuffer& vbo, uint32_t layout, uint32_t type, uint32_t components, uint32_t stride, uint32_t offset) const;
	void Bind() const;
	void UnBind() const;
	void Delete() const;

private:
	uint32_t rendererID;
};

