#include "Renderer/Shader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ogl
{

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
    // 从文件读取顶点着色器和片元着色器源码
    const std::string vertexSource = ReadFile(vertexPath);
    const std::string fragmentSource = ReadFile(fragmentPath);

    // 分别编译顶点着色器和片元着色器
    const unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    const unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // 创建 Shader Program 并把两个 shader 对象链接到一起
    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);
    glLinkProgram(m_program);

    // 检查 Shader Program 链接是否成功
    int success = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(m_program, sizeof(infoLog), nullptr, infoLog);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_program);
        m_program = 0;

        throw std::runtime_error(std::string("Failed to link shader program: ") + infoLog);
    }

    // Program 链接成功后，单独的 shader 对象已经不再需要
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
    // 释放 OpenGL Shader Program
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

void Shader::Bind() const
{
    // 让后续 Draw Call 使用当前 Shader Program
    glUseProgram(m_program);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &value) const
{
    // 根据名字找到shader里的uniform变量位置
    const int location = glGetUniformLocation(m_program, name.c_str());

    // 把GLM矩阵的数据上传给当前Shader Program
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    // 根据名字找到shader里的uniform变量位置
    const int location = glGetUniformLocation(m_program, name.c_str());

    // 把GLM三维向量的数据上传给当前Shader Program
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::SetFloat(const std::string &name, float value) const
{
    // 根据名字找到shader里的uniform变量位置
    const int location = glGetUniformLocation(m_program, name.c_str());

    // 把一个float数值上传给当前Shader Program
    glUniform1f(location, value);
}

void Shader::Unbind()
{
    // 解绑当前 Shader Program
    glUseProgram(0);
}

std::string Shader::ReadFile(const std::string &path)
{
    // 以文本方式打开 shader 源码文件
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + path);
    }

    // 将整个文件内容读入字符串
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    // 创建指定类型的 OpenGL shader 对象
    const unsigned int shader = glCreateShader(type);

    // 将 GLSL 源码传给 OpenGL
    const char *sourceText = source.c_str();
    glShaderSource(shader, 1, &sourceText, nullptr);

    // 编译 GLSL 源码
    glCompileShader(shader);

    // 检查编译是否成功
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        glDeleteShader(shader);
        throw std::runtime_error(std::string("Failed to compile shader: ") + infoLog);
    }

    return shader;
}

} // namespace ogl