#include "Texture.h"
#include "../OGLUtils.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Texture::Texture()
{
    texture = 0;
    w = 0;
    h = 0;
    d = 0;
}

Texture::~Texture()
{
    Delete();
}

bool Texture::CreateFromFile(const char* filename)
{
    uint8_t* img = stbi_load(filename, &w, &h, &d, 0);

    if (img)
    {
        bool ret = CreateFromBuffer(img, w, h, d);
        stbi_image_free(img);

        return ret;
    }

    return false;
}

bool Texture::CreateFromBuffer(uint8_t* buffer, const int w, const int h, const int d)
{
    if (!buffer || w <= 0 || h <= 0 || d <= 0)
        return false;

    this->w = w;
    this->h = h;
    this->d = d;

    GLCall(glGenTextures(1, &texture));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    return true;
}

void Texture::Bind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
}

void Texture::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::Delete()
{
    GLCall(glDeleteTextures(1, &texture));
    texture = 0;
}
