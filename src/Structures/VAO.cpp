#include "VAO.h"

#include "../OGLUtils.hpp"

VAO::VAO()
{
    GLCall(glGenVertexArrays(1, &rendererID));
}

VAO::~VAO()
{
    Delete();
}

//formo il layout
void VAO::LinkVertexBuffer(VertexBuffer& vbo, uint32_t layout, uint32_t type, uint32_t components, uint32_t stride, uint32_t offset) const
{
    Bind();
    vbo.Bind();

    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, components, type, GL_FALSE, stride, (void*)offset);

    vbo.UnBind();
    UnBind();
}

void VAO::Bind() const
{
    GLCall(glBindVertexArray(rendererID));
}

void VAO::UnBind() const
{
    GLCall(glBindVertexArray(0));
}

void VAO::Delete() const
{
    GLCall(glDeleteVertexArrays(1, &rendererID));
}