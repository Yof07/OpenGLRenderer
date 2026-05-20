#version 430 core

// 阶段4的顶点着色器
// 接收顶点位置和颜色，用模型矩阵、观察矩阵、投影矩阵计算顶点最终位置

layout (location = 0) in vec3 aPosition;    // 模型某个顶点在模型空间中的坐标
layout (location = 1) in vec3 aColor;       // 模型某个顶点的颜色

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 vColor;

void main()
{
    // 将顶点颜色传给片元着色器，三角形内部的颜色会由OpenGL自动插值
    vColor = aColor;

    // 把顶点从模型空间依次变换到世界空间、观察空间、裁剪空间
    gl_Position = u_Projection * u_View * u_Model * vec4(aPosition, 1.0);
}
