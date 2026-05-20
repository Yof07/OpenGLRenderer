#include "Core/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <stdexcept>

namespace ogl
{

ImGuiLayer::ImGuiLayer(GLFWwindow *window) : m_window(window)
{
    // 确保传入的GLFW窗口有效
    if (!m_window)
    {
        throw std::runtime_error("ImGuiLayer requires a valid GLFWwindow");
    }

    // 检查ImGui版本是否和当前编译版本匹配
    IMGUI_CHECKVERSION();

    // 创建ImGui上下文
    ImGui::CreateContext();

    // 获取ImGui输入输出配置
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // 使用ImGui默认深色主题
    ImGui::StyleColorsDark();

    // 将ImGui接入当前GLFW窗口
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);

    // 将ImGui接入当前OpenGL环境
    ImGui_ImplOpenGL3_Init("#version 430");
}

ImGuiLayer::~ImGuiLayer()
{
    // 释放ImGui OpenGL后端资源
    ImGui_ImplOpenGL3_Shutdown();

    // 释放ImGui GLFW后端资源
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();
}

void ImGuiLayer::BeginFrame()
{
    // 开始一帧ImGui OpenGL后端状态更新
    ImGui_ImplOpenGL3_NewFrame();

    // 开始一帧ImGui GLFW输入更新
    ImGui_ImplGlfw_NewFrame();

    // 开始一帧ImGui UI构建
    ImGui::NewFrame();
}

void ImGuiLayer::EndFrame()
{
    // 结束UI构建并生成绘制数据
    ImGui::Render();

    // 使用OpenGL绘制ImGui生成的UI数据
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace ogl
