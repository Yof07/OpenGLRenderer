#pragma once

namespace ogl
{

// 管理OpenGL Vertex Array Object的封装类
class VertexArray
{
public:
    /** 创建OpenGL VAO对象 */
    VertexArray();

    /** 释放OpenGL VAO对象 */
    ~VertexArray();

    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    /** 绑定当前VAO对象 */
    void Bind() const;

    /** 解绑当前VAO对象 */
    static void Unbind();

    /** 配置一个顶点属性如何从当前VBO中读取数据 */
    void SetAttribute(unsigned int index, int size, unsigned int type, bool normalized, int stride,
                      unsigned long long offset) const;

    /** 获取OpenGL VAO对象ID */
    unsigned int ID() const { return m_id; }

private:
    // OpenGL VAO对象ID
    unsigned int m_id = 0;
};

} // namespace ogl
