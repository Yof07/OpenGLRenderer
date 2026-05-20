#pragma once

namespace ogl
{

// 管理OpenGL Buffer对象的封装类，用Buffer来管理OpenGL Buffer对象的生命周期和设置数值
class Buffer
{
public:
    /** 创建指定类型的OpenGL Buffer对象，explicit防止用赋值写法来隐式转换而产生误解，必须显式构造 */
    explicit Buffer(unsigned int target);

    /** 释放OpenGL Buffer对象 */
    ~Buffer();

    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    /** 绑定当前Buffer对象 */
    void Bind() const;

    /** 解绑当前Buffer对象 */
    void Unbind() const;

    /** 向当前Buffer对象上传数据 */
    void SetData(const void *data, unsigned int size, unsigned int usage) const;

    /** 获取OpenGL Buffer对象ID */
    unsigned int ID() const { return m_id; }

private:
    // OpenGL Buffer对象ID
    unsigned int m_id = 0;

    // Buffer绑定目标类型，例如GL_ARRAY_BUFFER或GL_ELEMENT_ARRAY_BUFFER，表示这个用作VBO还是其他类型
    unsigned int m_target = 0;
};

} // namespace ogl
