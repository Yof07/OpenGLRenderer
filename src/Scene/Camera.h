#pragma once

#include <glm/glm.hpp>

namespace ogl
{

// 表示观察3D场景的相机
class Camera
{
public:
    /** 获取观察矩阵，即从世界空间到相机观察空间 */
    glm::mat4 GetViewMatrix() const;

    /** 获取投影矩阵，即从观察空间到裁剪空间 */
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

public:
    // 相机在世界空间中的位置
    glm::vec3 position{0.0f, 0.0f, 3.0f};

    // 相机看向的世界空间坐标，观察方向可以由target - position算出
    glm::vec3 target{0.0f, 0.0f, 0.0f};

    // 相机画面中的上方方向，当前项目约定+Y是上方
    glm::vec3 up{0.0f, 1.0f, 0.0f};

    // 相机上下方向的视野角度，左右范围会根据窗口宽高比计算
    float fov = 45.0f;

    // 相机能看见的最近距离
    float nearPlane = 0.1f;

    // 相机能看见的最远距离
    float farPlane = 100.0f;
};

} // namespace ogl
