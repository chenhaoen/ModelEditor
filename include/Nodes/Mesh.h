#pragma once

#include <vector>

#include "Nodes/Exports.h"

#include "RenderingContextDriver/Commons.h"

enum class DrawMode {
    Indexed,    
    NonIndexed  
};

enum class PrimitiveType {
    Triangles,  
    Lines,     
    Points      
};

class NODES_API Mesh
{
public:
    Mesh();
    ~Mesh();

    DrawMode m_drawMode;
    PrimitiveType m_primitiveType;

    void setVertices(const std::vector<Vertex>& vertices);
    const std::vector<Vertex>& getVertices() const;

    void setIndices(const std::vector<uint32_t>& indices);
    const std::vector<uint32_t>& getIndices() const;

    bool isChanged();
    void setChanged(const bool value);

    void record();
private:

    void compile();

    void createVertexBuffer();
    void freeVertexBuffer();

    void createIndexBuffer();
    void freeIndexBuffer();
private:

    bool m_isChanged;

    bool m_isCompiled;

    std::vector<Vertex> m_vertices;
    BufferID m_vertexBuffer;

    std::vector<uint32_t> m_indices;
    BufferID m_indexBuffer;
};

