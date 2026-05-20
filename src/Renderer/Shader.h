#pragma once

#include <glm/glm.hpp>

#include <string>

namespace ogl
{

// 管理 OpenGL Shader Program 的封装类
class Shader
{
public:
    /** 从顶点着色器和片元着色器文件创建 Shader Program */
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    /** 释放 OpenGL Shader Program */
    ~Shader();

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    /** 绑定当前 Shader Program，即告诉OpenGL接下来绘制使用当前ShaderProgram */
    void Bind() const;

    /** 给shader中的mat4 uniform变量传入矩阵 */
    void SetMat4(const std::string &name, const glm::mat4 &value) const;

    /** 给shader中的vec3 uniform变量传入三维向量 */
    void SetVec3(const std::string &name, const glm::vec3 &value) const;

    /** 给shader中的float uniform变量传入浮点数 */
    void SetFloat(const std::string &name, float value) const;

    /** 解绑当前 Shader Program */
    static void Unbind();

private:
    /** 读取 shader 源码文件 */
    static std::string ReadFile(const std::string &path);

    /** 根据 shader 类型将 GLSL 源码编译为 OpenGL shader 对象 */
    static unsigned int CompileShader(unsigned int type, const std::string &source);

private:
    // OpenGL Shader Program 对象 ID
    unsigned int m_program = 0;
};

} // namespace ogl
