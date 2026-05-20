#version 430 core

// 阶段6的光照顶点着色器，为光照计算准备数据的顶点着色器
// 接收模型顶点位置、颜色和法线，计算世界空间位置和世界空间法线

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

// 核心输出
out vec3 vWorldPosition;    // 世界空间位置
out vec3 vNormal;           // 世界空间法线
out vec3 vColor;

void main()
{
    // 先把顶点位置从模型空间变换到世界空间
    vec4 worldPosition = u_Model * vec4(aPosition, 1.0);

    // 传给片元着色器，用于后续计算光照方向和视线方向
    vWorldPosition = worldPosition.xyz;

    // 将法线从模型空间变换到世界空间
    vNormal = normalize(mat3(transpose(inverse(u_Model))) * aNormal);

    // 保留顶点颜色，后续作为物体基础颜色
    vColor = aColor;

    // 把世界空间位置继续变换到裁剪空间
    gl_Position = u_Projection * u_View * worldPosition;
}
