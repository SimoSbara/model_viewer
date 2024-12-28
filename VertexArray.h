#pragma once

#include "VertexBuffer.h"

#include <stdint.h>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);

private:
	uint32_t vaoID;
};

