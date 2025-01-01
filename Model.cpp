#include "Model.h"
#include "OGLUtils.hpp"

#define FAST_OBJ_IMPLEMENTATION
#include "fast_obj/fast_obj.h"

#include <fstream>
#include <vector>

Model::Model()
{

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
    if(mesh->color_count > 0)
        for (int i = 3, j = 0; j < mesh->color_count - 1; i += 3, j++)
        {
            VertexData& vertex = vertices[j];

            vertex.r = mesh->colors[i];
            vertex.g = mesh->colors[i + 1];
            vertex.b = mesh->colors[i + 2];
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


    //char buf[512];
    //float x, y, z, u, v;
    //uint32_t v1, v2, v3;
    //uint32_t vt1, vt2, vt3;

    //uint32_t posCount = 0;
    //uint32_t uvCount = 0;
    //uint32_t faceCount = 0;

    //std::string objLine;
    //std::ifstream objStream(filename, std::ios::in);

    //std::vector<VertexData> vertices;
    //std::vector<UVData> uvs;
    //std::vector<Indices> indices;
    //std::vector<Indices> uvIndices;

    //vertices.reserve(100);
    //uvs.reserve(100);
    //indices.reserve(100);
    //uvIndices.reserve(100);

    //if (!objStream.is_open())
    //    return false;

    //while (objStream.getline(buf, 512))
    //{
    //    objLine = buf;

    //    if (objLine.find("#", 0) != std::string::npos)
    //        continue;

    //    if (objLine.find("o", 0) != std::string::npos)
    //        continue;

    //    if (objLine.find("v ", 0) != std::string::npos)
    //    {
    //        sscanf_s(objLine.data(), "v %f %f %f", &x, &y, &z);

    //        while (vertices.size() <= posCount)
    //        {
    //            vertices.push_back(VertexData{});
    //        }

    //        vertices[posCount].x = x;
    //        vertices[posCount].y = y;
    //        vertices[posCount].z = z;
    //        vertices[posCount].r = (float)(rand() % 256) / 255.0f;
    //        vertices[posCount].g = (float)(rand() % 256) / 255.0f;
    //        vertices[posCount].b = (float)(rand() % 256) / 255.0f;
    //        vertices[posCount].uv.u = 0.0f;
    //        vertices[posCount].uv.v = 0.0f;

    //        posCount++;
    //    }
    //    else if (objLine.find("vt ", 0) != std::string::npos)
    //    {
    //        sscanf_s(objLine.data(), "vt %f %f", &u, &v);

    //        while (uvs.size() <= uvCount)
    //        {
    //            uvs.push_back(UVData{});
    //        }

    //        uvs[uvCount].u = u;
    //        uvs[uvCount].v = v;

    //        uvCount++;
    //    }
    //    else if (objLine.find("f ", 0) != std::string::npos)
    //    {
    //        //for (int i = 0; i < objLine.length() - 1; i++)
    //        //{
    //        //    if (objLine[i] == '/' && objLine[i + 1] == '/')
    //        //        objLine.replace(i, 2, " ", 1);
    //        //}

    //        //sscanf_s(objLine.data(), "f %lu/%lu %lu/%lu %lu/%lu", &v1, &vt1, &v2, &vt2, &v3, &vt3);
    //        sscanf_s(objLine.data(), "f %lu %lu %lu", &v1, &v2, &v3);

    //        v1--;
    //        v2--;
    //        v3--;

    //        //vt1--;
    //        //vt2--;
    //        //vt3--;

    //        indices.push_back(Indices{ v1, v2, v3 });
    //        //uvIndices.push_back(Indices{ vt1, vt2, vt3 });
    //    }   
    //}

    //int verticesSize = vertices.size();
    //int uvsSize = uvs.size();

    //for (int i = 0; i < uvIndices.size(); i++)
    //{
    //    Indices& uv = uvIndices[i];
    //    Indices& face = indices[i];

    //    if (face.i1 < verticesSize && uv.i1 < uvsSize)
    //    {
    //        vertices[face.i1].uv.u = uvs[uv.i1].u;
    //        vertices[face.i1].uv.v = uvs[uv.i1].v;
    //    }

    //    if (face.i2 < verticesSize && uv.i2 < uvsSize)
    //    {
    //        vertices[face.i2].uv.u = uvs[uv.i2].u;
    //        vertices[face.i2].uv.v = uvs[uv.i2].v;
    //    }

    //    if (face.i3 < verticesSize && uv.i3 < uvsSize)
    //    {
    //        vertices[face.i3].uv.u = uvs[uv.i3].u;
    //        vertices[face.i3].uv.v = uvs[uv.i3].v;
    //    }
    //}

    //vao.reset(new VAO);
    //vbo.reset(new VertexBuffer(vertices.data(), vertices.size() * sizeof(VertexData)));
    //ebo.reset(new IndexBuffer((uint32_t*)indices.data(), indices.size() * sizeof(uint32_t) * 3));

    //vao->LinkVertexBuffer(*vbo, 0, GL_FLOAT, 3, 8 * sizeof(float), 0);
    //vao->LinkVertexBuffer(*vbo, 1, GL_FLOAT, 3, 8 * sizeof(float), 3 * sizeof(float));
    //vao->LinkVertexBuffer(*vbo, 2, GL_FLOAT, 2, 8 * sizeof(float), 6 * sizeof(float));

    //vao->UnBind();
    //vbo->UnBind();
    //ebo->UnBind();

    //objStream.close();

    return true;
}

bool Model::CreateFromData(const float* vertexBuffer, const uint32_t vertexBufferSize, const uint32_t* indices, const uint32_t indicesSize)
{
    return false;
}

uint32_t Model::GetElementsCount() const
{
    if (!ebo)
        return 0;

    return ebo->GetElementsCount();
}

void Model::Bind() const
{
    if (!vao || !ebo || !vbo)
        return;

    vao->Bind();
    ebo->Bind();
}

void Model::UnBind() const
{
    if (!vao || !ebo || !vbo)
        return;

    vao->UnBind();
    ebo->UnBind();
}

void Model::Delete()
{
    vao.reset();
    vbo.reset();
    ebo.reset();
}