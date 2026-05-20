#version 430 core

// 阶段 1 的片元着色器
// 接收顶点着色器传来的插值颜色，并将它输出为屏幕上的像素颜色

in vec3 vColor;

out vec4 FragColor;

void main()
{
    // 将插值后的 RGB 颜色输出到当前 framebuffer
    FragColor = vec4(vColor, 1.0);
}
