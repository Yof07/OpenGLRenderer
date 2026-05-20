#pragma once

struct GLFWwindow;

namespace ogl
{

// 管理Dear ImGui初始化和每帧绘制流程的封装类
class ImGuiLayer
{
public:
    /** 使用已有GLFW窗口创建ImGui上下文 */
    explicit ImGuiLayer(GLFWwindow *window);

    /** 释放ImGui上下文和后端资源 */
    ~ImGuiLayer();

    ImGuiLayer(const ImGuiLayer &) = delete;
    ImGuiLayer &operator=(const ImGuiLayer &) = delete;

    /** 开始一帧ImGui UI构建 */
    void BeginFrame();

    /** 结束一帧ImGui UI构建并提交绘制 */
    void EndFrame();

private:
    // ImGui接入的GLFW窗口
    GLFWwindow *m_window = nullptr;
};

} // namespace ogl
