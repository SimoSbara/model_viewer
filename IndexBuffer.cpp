#include "IndexBuffer.h"

#include "OGLUtils.hpp"

IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
{
    this->count = count;

    GLCall(glGenBuffers(1, &rendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
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