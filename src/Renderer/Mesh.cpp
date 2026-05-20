#include "Renderer/Mesh.h"

#include <glad/gl.h>

#include <cstddef>

namespace ogl
{

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
    : m_vertexBuffer(GL_ARRAY_BUFFER), m_indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
    SetupMesh(vertices, indices);
}

void Mesh::Draw() const
{
    m_vertexArray.Bind();

    // 根据EBO中的索引绘制三角形
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);

    VertexArray::Unbind();
}

void Mesh::SetupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices)
{
    // 记录索引数量，绘制时会传给glDrawElements
    m_indexCount = static_cast<int>(indices.size());

    m_vertexArray.Bind();

    // 上传顶点数据到VBO，上传索引数据到EBO
    m_vertexBuffer.SetData(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(Vertex)),
                           GL_STATIC_DRAW);
    m_indexBuffer.SetData(indices.data(), static_cast<unsigned int>(indices.size() * sizeof(unsigned int)),
                          GL_STATIC_DRAW);

    // location 0对应顶点位置
    m_vertexArray.SetAttribute(0, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, position));

    // location 1对应顶点颜色
    m_vertexArray.SetAttribute(1, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, color));

    // location 2对应顶点法线，后续光照阶段会使用
    m_vertexArray.SetAttribute(2, 3, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, normal));

    // location 3对应纹理坐标，后续贴图阶段会使用
    m_vertexArray.SetAttribute(3, 2, GL_FLOAT, false, sizeof(Vertex), offsetof(Vertex, texCoord));

    m_vertexBuffer.Unbind();
    VertexArray::Unbind();
}

} // namespace ogl
