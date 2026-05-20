#include "Core/Window.h"

#include <glad/gl.h>

#include <stdexcept>

namespace ogl
{

Window::Window(int width, int height, const std::string &title)
    : m_width(width), m_height(height)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    // 记录Window指针并注册窗口相关回调
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, FramebufferSizeCallback);
    glfwSetScrollCallback(m_window, ScrollCallback);

    // 开启垂直同步，避免渲染循环跑得过快
    glfwSwapInterval(1);
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::SetShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(m_window, shouldClose ? GLFW_TRUE : GLFW_FALSE);
}

void Window::SwapBuffers() { glfwSwapBuffers(m_window); }

double Window::ConsumeScrollOffsetY()
{
    // 取出当前累计滚轮偏移并清零，保证同一次滚轮输入只消费一次
    const double scrollOffsetY = m_scrollOffsetY;
    m_scrollOffsetY = 0.0;
    return scrollOffsetY;
}

void Window::PollEvents() { glfwPollEvents(); }

void Window::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // 从GLFW窗口取回Window对象并同步窗口尺寸
    auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self)
    {
        self->m_width = width;
        self->m_height = height;
    }

    // 同步OpenGL视口尺寸
    glViewport(0, 0, width, height);
}

void Window::ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    // 当前只使用Y方向滚轮偏移，X方向偏移先忽略
    (void)xOffset;

    // 从GLFW窗口取回Window对象并累计滚轮偏移
    auto *self = static_cast<Window *>(glfwGetWindowUserPointer(window));
    if (self)
    {
        self->m_scrollOffsetY += yOffset;
    }
}

} // namespace ogl
