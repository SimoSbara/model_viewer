#pragma once

#include <stdint.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
	void Delete() const;

private:
	uint32_t rendererID;
};