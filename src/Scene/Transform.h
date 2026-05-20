#pragma once

#include <glm/glm.hpp>

namespace ogl
{
/** 保存一个物体的位置、旋转和缩放数据，并能生成模型矩阵(把模型从模型空间转换到世界空间的矩阵) */
class Transform
{
public:
    /** 根据位置、旋转、缩放生成模型矩阵 */
    glm::mat4 GetModelMatrix() const;

    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::vec3 rotation{0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
};
} // namespace ogl
