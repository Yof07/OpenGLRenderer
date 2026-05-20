#include "Scene/CameraController.h"

#include "Scene/Camera.h"

#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <imgui.h>

#include <stdexcept>

namespace ogl
{

CameraController::CameraController(Camera &camera, GLFWwindow *window) : m_camera(camera), m_window(window)
{
    // 检查传入的GLFW窗口是否有效
    if (!m_window)
    {
        throw std::runtime_error("CameraController requires a valid GLFWwindow");
    }

    // 根据初始yaw和pitch同步相机方向
    UpdateCameraVectors();
}

void CameraController::Update(float deltaTime)
{
    // 先处理鼠标旋转，再根据新的角度更新相机方向
    ProcessMouse();
    UpdateCameraVectors();

    // 再处理键盘移动，移动后重新设置观察目标
    ProcessKeyboard(deltaTime);
    m_camera.target = m_camera.position + m_front;
}

void CameraController::ProcessScroll(double yOffset)
{
    // 没有滚轮输入时不修改FOV
    if (yOffset == 0.0)
    {
        return;
    }

    // 鼠标正在操作ImGui时不让滚轮同时改变相机FOV
    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    // 根据滚轮方向调整FOV，向上滚动会缩小FOV并产生放大效果
    m_camera.fov -= static_cast<float>(yOffset) * m_scrollSensitivity;
    m_camera.fov = glm::clamp(m_camera.fov, 20.0f, 90.0f);
}

void CameraController::RenderDebugUI()
{
    // 显示相机调试区域标题
    ImGui::Separator();
    ImGui::Text("Camera");

    // 调节相机基础参数和控制器速度参数
    ImGui::DragFloat3("Camera Position", &m_camera.position.x, 0.05f, -50.0f, 50.0f);
    ImGui::SliderFloat("Camera FOV", &m_camera.fov, 20.0f, 90.0f);
    ImGui::SliderFloat("Camera Near", &m_camera.nearPlane, 0.01f, 10.0f);
    ImGui::SliderFloat("Camera Far", &m_camera.farPlane, 10.0f, 500.0f);
    ImGui::SliderFloat("Camera Move Speed", &m_moveSpeed, 0.1f, 20.0f);
    ImGui::SliderFloat("Mouse Sensitivity", &m_mouseSensitivity, 0.01f, 1.0f);
    ImGui::SliderFloat("Scroll Sensitivity", &m_scrollSensitivity, 0.1f, 10.0f);

    // 保证远裁剪面始终大于近裁剪面
    if (m_camera.farPlane <= m_camera.nearPlane)
    {
        m_camera.farPlane = m_camera.nearPlane + 1.0f;
    }

    // 显示当前视角角度，方便观察鼠标输入结果
    ImGui::Text("Yaw: %.1f", m_yaw);
    ImGui::Text("Pitch: %.1f", m_pitch);
}

void CameraController::UpdateCameraVectors()
{
    // 根据yaw和pitch计算相机朝前方向
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // 根据朝前方向和世界上方方向计算相机朝右方向
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));

    // 同步Camera实际用于生成View矩阵的数据
    m_camera.up = m_worldUp;
    m_camera.target = m_camera.position + m_front;
}

void CameraController::ProcessKeyboard(float deltaTime)
{
    // 根据帧时间计算本帧移动距离
    const float velocity = m_moveSpeed * deltaTime;

    // 根据W和S沿相机朝前方向前后移动
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_camera.position += m_front * velocity;
    }
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_camera.position -= m_front * velocity;
    }

    // 根据A和D沿相机朝右方向左右移动
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_camera.position -= m_right * velocity;
    }
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_camera.position += m_right * velocity;
    }
}

void CameraController::ProcessMouse()
{
    // 读取左键和ImGui鼠标占用状态
    const bool leftMousePressed = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    const bool imguiWantsMouse = ImGui::GetIO().WantCaptureMouse;

    // 左键未按下或鼠标正在操作ImGui时退出视角旋转
    if (!leftMousePressed || imguiWantsMouse)
    {
        if (m_mouseLookEnabled)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        m_mouseLookEnabled = false;
        m_firstMouse = true;
        return;
    }

    // 第一次进入视角旋转时隐藏并捕获鼠标
    if (!m_mouseLookEnabled)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_mouseLookEnabled = true;
    }

    // 读取当前鼠标位置
    double mouseX = 0.0;
    double mouseY = 0.0;
    glfwGetCursorPos(m_window, &mouseX, &mouseY);

    // 第一次读取鼠标时只记录位置，避免产生突然跳变
    if (m_firstMouse)
    {
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        m_firstMouse = false;
        return;
    }

    // 计算本帧鼠标偏移并更新上一帧位置
    float xOffset = static_cast<float>(mouseX - m_lastMouseX);
    float yOffset = static_cast<float>(m_lastMouseY - mouseY);
    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;

    // 根据鼠标灵敏度更新yaw和pitch
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;
    m_yaw += xOffset;
    m_pitch += yOffset;

    // 限制pitch避免相机翻转
    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);
}

} // namespace ogl
