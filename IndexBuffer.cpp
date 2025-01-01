#include "IndexBuffer.h"

#include "OGLUtils.hpp"

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t size)
{
    this->count = size / sizeof(uint32_t);

    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    Delete();
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::UnBind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &rendererID));
}