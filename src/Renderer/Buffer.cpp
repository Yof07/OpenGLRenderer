#include "Renderer/Buffer.h"

#include <glad/gl.h>

namespace ogl
{

Buffer::Buffer(unsigned int target) : m_target(target)
{
    // 创建OpenGL Buffer对象并保存它的ID
    glGenBuffers(1, &m_id);
}

Buffer::~Buffer()
{
    // 释放OpenGL Buffer对象
    if (m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

void Buffer::Bind() const
{
    // 将当前Buffer绑定到它记录的目标槽位上
    glBindBuffer(m_target, m_id);
}

void Buffer::Unbind() const
{
    // 解绑当前Buffer绑定的目标槽位
    glBindBuffer(m_target, 0);
}

void Buffer::SetData(const void *data, unsigned int size, unsigned int usage) const
{
    // 先绑定当前Buffer，保证数据上传到正确的OpenGL Buffer对象
    Bind();

    // 将CPU侧数据上传到当前绑定的Buffer
    glBufferData(m_target, size, data, usage);
}

} // namespace ogl
