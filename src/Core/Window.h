#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>

namespace ogl
{

class Window
{
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    bool ShouldClose() const;
    void SetShouldClose(bool shouldClose);

    /** OpenGL窗口双缓冲，把后台缓冲区的画面显示到屏幕上 */
    void SwapBuffers();

    int Width() const { return m_width; }
    int Height() const { return m_height; }
    GLFWwindow *NativeHandle() const { return m_window; }

    /** 取出本帧累计的鼠标滚轮Y方向偏移并清零 */
    double ConsumeScrollOffsetY();

    /** 让 GLFW 处理系统窗口事件和输入事件，比如键盘、鼠标、窗口关闭和 resize */
    static void PollEvents();

private:
    /** 实际画面尺寸发生变化时的回调 */
    static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

    /** 鼠标滚轮滚动时的回调 */
    static void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

private:
    // GLFW窗口对象
    GLFWwindow *m_window = nullptr;

    // 当前窗口宽度
    int m_width = 0;

    // 当前窗口高度
    int m_height = 0;

    // 鼠标滚轮Y方向累计偏移
    double m_scrollOffsetY = 0.0;
};

} // namespace ogl
