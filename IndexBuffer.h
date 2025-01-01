#pragma once

#include <stdint.h>

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* data, uint32_t size);
	~IndexBuffer();

	inline uint32_t GetElementsCount() const
	{
		return this->count;
	}

	void Bind() const;
	void UnBind() const;
	void Delete() const;

private:
	uint32_t rendererID;
	uint32_t count;
};

