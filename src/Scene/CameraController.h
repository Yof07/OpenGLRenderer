#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ogl
{

class Camera;

// 根据键盘和鼠标输入更新场景相机
class CameraController
{
public:
    /** 使用已有Camera和GLFW窗口创建相机控制器 */
    CameraController(Camera &camera, GLFWwindow *window);

    /** 每帧读取输入并更新Camera */
    void Update(float deltaTime);

    /** 根据鼠标滚轮调整Camera FOV */
    void ProcessScroll(double yOffset);

    /** 绘制相机参数调试控件 */
    void RenderDebugUI();

private:
    /** 根据yaw和pitch重新计算相机方向 */
    void UpdateCameraVectors();

    /** 处理WASD键盘移动 */
    void ProcessKeyboard(float deltaTime);

    /** 处理鼠标左键视角旋转 */
    void ProcessMouse();

private:
    // 被控制的场景相机
    Camera &m_camera;

    // 读取键盘和鼠标输入的GLFW窗口
    GLFWwindow *m_window = nullptr;

    // 相机当前朝前方向
    glm::vec3 m_front{0.0f, 0.0f, -1.0f};

    // 相机当前朝右方向
    glm::vec3 m_right{1.0f, 0.0f, 0.0f};

    // 世界空间上方方向
    glm::vec3 m_worldUp{0.0f, 1.0f, 0.0f};

    // 相机水平旋转角度
    float m_yaw = -90.0f;

    // 相机上下旋转角度
    float m_pitch = 0.0f;

    // 相机每秒移动距离
    float m_moveSpeed = 3.0f;

    // 鼠标移动对视角旋转的影响强度
    float m_mouseSensitivity = 0.12f;

    // 鼠标滚轮对FOV变化的影响强度
    float m_scrollSensitivity = 2.0f;

    // 是否是本次左键观察的第一帧鼠标输入
    bool m_firstMouse = true;

    // 上一帧鼠标X坐标
    double m_lastMouseX = 0.0;

    // 上一帧鼠标Y坐标
    double m_lastMouseY = 0.0;

    // 当前是否正在用鼠标左键旋转视角
    bool m_mouseLookEnabled = false;
};

} // namespace ogl
