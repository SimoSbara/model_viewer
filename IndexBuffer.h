#pragma once

#include <stdint.h>

class IndexBuffer
{
public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
	void Delete() const;

	inline uint32_t GetCount() const
	{
		return this->count;
	}

private:
	uint32_t rendererID;
	uint32_t count;
};

