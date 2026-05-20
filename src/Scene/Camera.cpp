#include "Scene/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace ogl
{

glm::mat4 Camera::GetViewMatrix() const
{
    // 根据相机位置、观察目标和上方向生成观察矩阵
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
    // 根据视野角度、宽高比、近裁剪面和远裁剪面生成透视投影矩阵
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

} // namespace ogl
