#pragma once

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <glm/glm.hpp>

#include <vector>

namespace ogl
{

// 表示模型中的一个顶点
struct Vertex
{
    // 顶点在模型空间中的位置
    glm::vec3 position{0.0f, 0.0f, 0.0f};

    // 顶点颜色，当前用于保留彩色三角形效果
    glm::vec3 color{1.0f, 1.0f, 1.0f};

    // 顶点法线，后续用于光照计算
    glm::vec3 normal{0.0f, 0.0f, 1.0f};

    // 顶点纹理坐标，后续用于贴图采样
    glm::vec2 texCoord{0.0f, 0.0f};
};

// 管理一组顶点数据和绘制命令的封装类
class Mesh
{
public:
    /** 根据顶点数组和索引数组创建Mesh */
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

    /** 绘制当前Mesh */
    void Draw() const;

private:
    /** 创建VAO、VBO、EBO并配置顶点属性布局 */
    void SetupMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);

private:
    // 记录顶点属性布局
    VertexArray m_vertexArray;

    // 保存顶点数据
    Buffer m_vertexBuffer;

    // 保存索引数据
    Buffer m_indexBuffer;

    // 当前Mesh包含的索引数量
    int m_indexCount = 0;
};

} // namespace ogl
