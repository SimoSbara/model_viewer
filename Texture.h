#pragma once

#include <stdint.h>

class Texture
{
public:
	Texture();
	~Texture();

	bool CreateFromFile(const char* filename);
	bool CreateFromBuffer(uint8_t* buffer, const int w, const int h, const int d);

	void Bind() const;
	void UnBind() const;
	void Delete();

private:
	uint32_t texture;
	int w, h, d;
};

