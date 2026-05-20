#pragma once

#include "Core/ImGuiLayer.h"
#include "Core/Window.h"
#include "Scene/Scene.h"

#include <glm/glm.hpp>

#include <memory>

namespace ogl
{

class CameraController;
class Material;

class Application
{
public:
    Application();

    /** 释放OpenGL资源 */
    ~Application();

    /** 运行应用主循环 */
    void Run();

private:
    /** 初始化GLAD、viewport和基础OpenGL状态 */
    void InitializeOpenGL();

    /** 处理输入 */
    void ProcessInput();

    /** 渲染当前帧 */
    void RenderFrame();

    /** 渲染ImGui调试面板 */
    void RenderDebugUI();

    /** 打印当前OpenGL环境信息 */
    void PrintOpenGLInfo() const;

    /** 创建场景中的测试对象 */
    void CreateSceneObjects();

private:
    /* 窗口和UI资源 */

    // 应用窗口
    Window m_window;

    // ImGui调试面板
    std::unique_ptr<ImGuiLayer> m_imguiLayer;

    /* 帧时间状态 */

    // 上一帧开始时的时间
    float m_lastFrameTime = 0.0f;

    // 当前帧和上一帧之间经过的时间
    float m_deltaTime = 0.0f;

    /* 场景和相机交互 */

    // 当前要渲染的场景
    Scene m_scene;

    // 交互相机控制器
    std::unique_ptr<CameraController> m_cameraController;

    /* 材质资源 */

    // 当前OBJ模型使用的材质，阶段6先用它上传光照uniform
    std::shared_ptr<Material> m_modelMaterial;

    /* 基础渲染调试参数 */

    // 清屏颜色
    float m_clearColor[3] = {0.08f, 0.10f, 0.13f};

    // 当前物体基础颜色
    glm::vec3 m_objectColor{1.0f, 1.0f, 1.0f};

    // 环境光强度
    float m_ambientStrength = 0.18f;

    // 高光强度
    float m_specularStrength = 0.45f;

    // 高光指数，数值越大高光越集中
    float m_shininess = 32.0f;

    /* 方向光参数 */

    // 当前方向光的照射方向
    glm::vec3 m_lightDirection{-0.3f, -1.0f, -0.4f};

    // 当前方向光颜色
    glm::vec3 m_lightColor{1.0f, 1.0f, 1.0f};

    /* 点光源参数 */

    // 当前点光源在世界空间中的位置
    glm::vec3 m_pointLightPosition{1.5f, 1.0f, 2.0f};

    // 当前点光源颜色
    glm::vec3 m_pointLightColor{1.0f, 0.75f, 0.45f};

    // 点光源衰减公式中的常数项
    float m_pointLightConstant = 1.0f;

    // 点光源衰减公式中的一次项
    float m_pointLightLinear = 0.09f;

    // 点光源衰减公式中的二次项
    float m_pointLightQuadratic = 0.032f;

    /* 聚光灯参数 */

    // 当前聚光灯在世界空间中的位置
    glm::vec3 m_spotLightPosition{0.0f, 0.0f, 3.0f};

    // 当前聚光灯的照射方向
    glm::vec3 m_spotLightDirection{0.0f, 0.0f, -1.0f};

    // 当前聚光灯颜色
    glm::vec3 m_spotLightColor{0.4f, 0.7f, 1.0f};

    // 聚光灯内圈角度，角度内保持完整亮度
    float m_spotLightCutOff = 12.5f;

    // 聚光灯外圈角度，内圈到外圈之间逐渐变暗
    float m_spotLightOuterCutOff = 17.5f;

    // 聚光灯衰减公式中的常数项
    float m_spotLightConstant = 1.0f;

    // 聚光灯衰减公式中的一次项
    float m_spotLightLinear = 0.09f;

    // 聚光灯衰减公式中的二次项
    float m_spotLightQuadratic = 0.032f;
};

} // namespace ogl