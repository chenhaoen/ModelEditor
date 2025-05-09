#pragma once

#include "Core/Exports.h"
#include "Core/Compilable.h"

#include "Core/RenderingContextDriver/Commons.h"

class CORE_API Mesh : public Compilable
{
public:
    Mesh();
    ~Mesh() override;

    DrawMode m_drawMode;
    PrimitiveType m_primitiveType;

    void setVertices(const std::vector<Vertex>& vertices);
    std::vector<Vertex>& getVertices();
    const std::vector<Vertex>& getVertices() const;

    void setIndices(const std::vector<uint32_t>& indices);
    std::vector<uint32_t>& getIndices();
    const std::vector<uint32_t>& getIndices() const;

    bool isChanged();
    void setChanged(const bool value);

    void compile() override;

    void record() override;
private:

    void createVertexBuffer();
    void freeVertexBuffer();

    void createIndexBuffer();
    void freeIndexBuffer();
private:
    bool m_isChanged;

    std::vector<Vertex> m_vertices;
    BufferID m_vertexBuffer;

    std::vector<uint32_t> m_indices;
    BufferID m_indexBuffer;
};

