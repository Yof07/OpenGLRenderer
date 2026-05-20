#include "Renderer/VertexArray.h"

#include <glad/gl.h>

namespace ogl
{

VertexArray::VertexArray()
{
    // 创建OpenGL VAO对象并保存它的ID
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    // 释放OpenGL VAO对象
    if (m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

void VertexArray::Bind() const
{
    // 绑定当前VAO对象
    glBindVertexArray(m_id);
}

void VertexArray::Unbind()
{
    // 解绑当前VAO对象
    glBindVertexArray(0);
}

void VertexArray::SetAttribute(unsigned int index, int size, unsigned int type, bool normalized, int stride,
                               unsigned long long offset) const
{
    // 配置当前VAO中指定location的顶点属性读取方式
    glVertexAttribPointer(index, size, type, normalized ? GL_TRUE : GL_FALSE, stride,
                          reinterpret_cast<const void *>(offset));

    // 启用当前location，让Vertex Shader可以读取这个属性
    glEnableVertexAttribArray(index);
}

} // namespace ogl
