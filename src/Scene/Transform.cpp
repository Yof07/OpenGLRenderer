#include "Scene/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ogl
{
glm::mat4 Transform::GetModelMatrix() const
{
    // 从单位矩阵开始，表示还没有任何变换
    glm::mat4 model(1.0f);

    // 先平移，把物体移动到世界空间中的指定位置
    model = glm::translate(model, position);

    // 再旋转，rotation里保存的是角度，glm::rotate需要弧度
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // 最后缩放，改变物体在三个轴上的大小
    model = glm::scale(model, scale);

    return model;
}
} // namespace ogl
