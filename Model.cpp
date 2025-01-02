#include "Model.h"
#include "OGLUtils.hpp"

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj/fast_obj.h"

#include <fstream>
#include <vector>

Model::Model()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
}

Model::~Model()
{
    Delete();
}

bool Model::CreateFromOBJ(const char* filename)
{
    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;

    fastObjMesh* mesh = fast_obj_read(filename);

    if (!mesh)
        return false;

    vertices.resize(mesh->position_count - 1);
    indices.resize(mesh->index_count);

    //carico posizioni
    if (mesh->position_count > 0)
        for (int i = 3, j = 0; j < mesh->position_count - 1; i += 3, j++)
        {
            VertexData& vertex = vertices[j];

            vertex.x = mesh->positions[i];
            vertex.y = mesh->positions[i + 1];
            vertex.z = mesh->positions[i + 2];
        }

    //carico colori
    if (mesh->color_count > 0)
    {
        for (int i = 3, j = 0; j < mesh->color_count - 1; i += 3, j++)
        {
            VertexData& vertex = vertices[j];

            vertex.r = mesh->colors[i];
            vertex.g = mesh->colors[i + 1];
            vertex.b = mesh->colors[i + 2];
        }
    }
    else //genero io
    {
        for (int i = 0; i < vertices.size(); i++)
        {
            VertexData& vertex = vertices[i];

            vertex.r = (float)(rand() % 256) / 255.0f;
            vertex.g = (float)(rand() % 256) / 255.0f;
            vertex.b = (float)(rand() % 256) / 255.0f;
        }
    }

    //carico UV e normali secondo gli indici
    for (int i = 0; i < mesh->index_count; i += 3)
    {
        fastObjIndex* index = &mesh->indices[i];
        
        for (int j = 0; j < 3; j++, index++)
        {
            uint32_t p = index->p - 1;
            uint32_t& n = index->n;
            uint32_t t = index->t - 1;

            VertexData& vertex = vertices[p];

            indices[i + j] = p;

            if (mesh->texcoord_count > 2)
            {
                vertex.u = mesh->texcoords[t];
                vertex.v = mesh->texcoords[t + 1];
            }

            if (mesh->normal_count > 3)
            {
                vertex.nx = mesh->normals[n];
                vertex.ny = mesh->normals[n + 1];
                vertex.nz = mesh->normals[n + 2];
            }
        }
    }

    vao.reset(new VAO);
    vbo.reset(new VertexBuffer(vertices.data(), vertices.size() * sizeof(VertexData)));
    ebo.reset(new IndexBuffer((uint32_t*)indices.data(), indices.size() * sizeof(uint32_t)));

    vao->LinkVertexBuffer(*vbo, 0, GL_FLOAT, 3, 11 * sizeof(float), 0); //pos
    vao->LinkVertexBuffer(*vbo, 1, GL_FLOAT, 3, 11 * sizeof(float), 3 * sizeof(float)); //colors
    vao->LinkVertexBuffer(*vbo, 2, GL_FLOAT, 2, 11 * sizeof(float), 6 * sizeof(float)); //uv
    vao->LinkVertexBuffer(*vbo, 3, GL_FLOAT, 3, 11 * sizeof(float), 8 * sizeof(float)); //normals

    vao->UnBind();
    vbo->UnBind();
    ebo->UnBind();
   
    fast_obj_destroy(mesh);

    return true;
}

bool Model::LoadTexture(const char* filename)
{
    texture.reset(new Texture);

    return texture->CreateFromFile(filename);
}

void Model::UpdatePosition(const glm::vec3 position)
{
    this->position = position;
    this->model = glm::translate(glm::mat4(1.0f), position);
}

//solo asse Y per ora
void Model::UpdatePositionRotation(const glm::vec3 position, const float angle)
{
    UpdatePosition(position);
    this->model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::SetModelMatrix(const int& shader) const
{
    GLCall(glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model)));
}

void Model::Draw(const bool& useTexture) const
{
    if (!vao || !ebo || !vbo)
        return;

    vao->Bind();
    ebo->Bind();

    if (useTexture && texture)
        texture->Bind();

    glDrawElements(GL_TRIANGLES, GetElementsCount(), GL_UNSIGNED_INT, 0);

    if (useTexture && texture)
        texture->UnBind();

    vao->UnBind();
    ebo->UnBind();
}

void Model::Delete()
{
    vao.reset();
    vbo.reset();
    ebo.reset();
}