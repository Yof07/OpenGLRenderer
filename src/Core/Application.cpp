#include "Core/Application.h"

#include "Renderer/Material.h"
#include "Renderer/Model.h"
#include "Renderer/Shader.h"
#include "Scene/CameraController.h"
#include "Scene/GameObject.h"
#include "Scene/RenderComponent.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>
#include <glm/trigonometric.hpp>
#include <imgui.h>

#include <iostream>
#include <memory>
#include <stdexcept>

namespace ogl
{

Application::Application() : m_window(1280, 720, "OpenGLRenderer")
{
    // 初始化OpenGL环境并打印当前显卡和版本信息
    InitializeOpenGL();
    PrintOpenGLInfo();

    // 在OpenGL初始化完成后创建ImGui层
    m_imguiLayer = std::make_unique<ImGuiLayer>(m_window.NativeHandle());

    // 创建场景对象和交互相机控制器
    CreateSceneObjects();
    m_cameraController = std::make_unique<CameraController>(m_scene.GetCamera(), m_window.NativeHandle());

    // 记录第一帧开始前的时间，避免第一帧deltaTime过大
    m_lastFrameTime = static_cast<float>(glfwGetTime());
}

Application::~Application()
{
    // 在Window销毁前释放ImGui层
    m_imguiLayer.reset();
}

void Application::Run()
{
    while (!m_window.ShouldClose())
    {
        // 计算当前帧和上一帧之间经过的时间
        const float currentFrameTime = static_cast<float>(glfwGetTime());
        m_deltaTime = currentFrameTime - m_lastFrameTime;
        m_lastFrameTime = currentFrameTime;

        // 处理应用级输入，比如ESC关闭窗口
        ProcessInput();

        // 开始一帧ImGuiUI构建
        m_imguiLayer->BeginFrame();

        // 根据当前帧输入更新相机，再使用更新后的相机绘制场景
        if (m_cameraController)
        {
            m_cameraController->ProcessScroll(m_window.ConsumeScrollOffsetY());
            m_cameraController->Update(m_deltaTime);
        }
        RenderFrame();
        RenderDebugUI();

        // 提交并绘制ImGuiUI
        m_imguiLayer->EndFrame();

        // 显示当前帧并处理窗口事件
        m_window.SwapBuffers();
        Window::PollEvents();
    }
}

void Application::InitializeOpenGL()
{
    // 传入glfwGetProcAddress的函数地址，让GLAD加载OpenGL函数指针
    if (!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // 设置OpenGL渲染区域为当前窗口的大小
    glViewport(0, 0, m_window.Width(), m_window.Height());

    // 开启深度测试用于处理三维物体前后遮挡
    glEnable(GL_DEPTH_TEST);
}

void Application::ProcessInput()
{
    // 按ESC时请求关闭窗口
    if (glfwGetKey(m_window.NativeHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        m_window.SetShouldClose(true);
    }
}

void Application::RenderFrame()
{
    // 清空上一帧的颜色和深度数据
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 每帧上传当前光照参数，保证ImGui修改后下一帧生效
    if (m_modelMaterial)
    {
        m_modelMaterial->Bind();
        m_modelMaterial->SetVec3("u_ObjectColor", m_objectColor);
        m_modelMaterial->SetVec3("u_ViewPosition", m_scene.GetCamera().position);
        m_modelMaterial->SetVec3("u_LightDirection", m_lightDirection);
        m_modelMaterial->SetVec3("u_LightColor", m_lightColor);
        m_modelMaterial->SetVec3("u_PointLight.position", m_pointLightPosition);
        m_modelMaterial->SetVec3("u_PointLight.color", m_pointLightColor);
        m_modelMaterial->SetFloat("u_PointLight.constant", m_pointLightConstant);
        m_modelMaterial->SetFloat("u_PointLight.linear", m_pointLightLinear);
        m_modelMaterial->SetFloat("u_PointLight.quadratic", m_pointLightQuadratic);
        m_modelMaterial->SetVec3("u_SpotLight.position", m_spotLightPosition);
        m_modelMaterial->SetVec3("u_SpotLight.direction", m_spotLightDirection);
        m_modelMaterial->SetVec3("u_SpotLight.color", m_spotLightColor);
        m_modelMaterial->SetFloat("u_SpotLight.cutOff", glm::cos(glm::radians(m_spotLightCutOff)));
        m_modelMaterial->SetFloat("u_SpotLight.outerCutOff", glm::cos(glm::radians(m_spotLightOuterCutOff)));
        m_modelMaterial->SetFloat("u_SpotLight.constant", m_spotLightConstant);
        m_modelMaterial->SetFloat("u_SpotLight.linear", m_spotLightLinear);
        m_modelMaterial->SetFloat("u_SpotLight.quadratic", m_spotLightQuadratic);
        m_modelMaterial->SetFloat("u_AmbientStrength", m_ambientStrength);
        m_modelMaterial->SetFloat("u_SpecularStrength", m_specularStrength);
        m_modelMaterial->SetFloat("u_Shininess", m_shininess);
    }

    // 根据当前窗口大小计算投影矩阵需要的宽高比
    const float aspectRatio = static_cast<float>(m_window.Width()) / static_cast<float>(m_window.Height());

    // 绘制当前场景
    m_scene.Draw(aspectRatio);
}

void Application::RenderDebugUI()
{
    // 创建一个调试窗口
    ImGui::Begin("OpenGLRenderer");

    // 显示当前帧率和清屏颜色
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::ColorEdit3("Clear Color", m_clearColor);

    // 显示并调节相机参数
    if (m_cameraController)
    {
        m_cameraController->RenderDebugUI();
    }

    // 调节物体基础颜色
    ImGui::Separator();
    ImGui::ColorEdit3("Object Color", &m_objectColor.x);

    // 调节方向光颜色和方向
    ImGui::ColorEdit3("Directional Light Color", &m_lightColor.x);
    ImGui::DragFloat3("Directional Light Direction", &m_lightDirection.x, 0.05f, -5.0f, 5.0f);

    // 调节点光源颜色、位置和衰减参数
    ImGui::Separator();
    ImGui::ColorEdit3("Point Light Color", &m_pointLightColor.x);
    ImGui::DragFloat3("Point Light Position", &m_pointLightPosition.x, 0.05f, -5.0f, 5.0f);
    ImGui::SliderFloat("Point Constant", &m_pointLightConstant, 0.1f, 2.0f);
    ImGui::SliderFloat("Point Linear", &m_pointLightLinear, 0.0f, 1.0f);
    ImGui::SliderFloat("Point Quadratic", &m_pointLightQuadratic, 0.0f, 1.0f);

    // 调节聚光灯颜色、位置、方向和角度
    ImGui::Separator();
    ImGui::ColorEdit3("Spot Light Color", &m_spotLightColor.x);
    ImGui::DragFloat3("Spot Light Position", &m_spotLightPosition.x, 0.05f, -5.0f, 5.0f);
    ImGui::DragFloat3("Spot Light Direction", &m_spotLightDirection.x, 0.05f, -5.0f, 5.0f);
    ImGui::SliderFloat("Spot CutOff", &m_spotLightCutOff, 1.0f, 45.0f);
    ImGui::SliderFloat("Spot Outer CutOff", &m_spotLightOuterCutOff, 1.0f, 60.0f);

    // 调节聚光灯衰减参数
    ImGui::SliderFloat("Spot Constant", &m_spotLightConstant, 0.1f, 2.0f);
    ImGui::SliderFloat("Spot Linear", &m_spotLightLinear, 0.0f, 1.0f);
    ImGui::SliderFloat("Spot Quadratic", &m_spotLightQuadratic, 0.0f, 1.0f);

    // 调节Blinn-Phong光照参数
    ImGui::Separator();
    ImGui::SliderFloat("Ambient Strength", &m_ambientStrength, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular Strength", &m_specularStrength, 0.0f, 2.0f);
    ImGui::SliderFloat("Shininess", &m_shininess, 1.0f, 128.0f);

    // 显示当前补齐内容
    ImGui::Text("Stage 4 Patch: CameraController + Stage 6 Lighting");

    ImGui::End();
}

void Application::PrintOpenGLInfo() const
{
    // 打印当前OpenGL驱动和版本信息
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
}

void Application::CreateSceneObjects()
{
    // 从OBJ文件加载模型
    auto cubeModel = std::make_shared<Model>("assets/models/cube.obj");

    // 创建模型使用的Shader和Material
    auto cubeShader = std::make_shared<Shader>("shaders/lit.vert", "shaders/lit.frag");
    auto cubeMaterial = std::make_shared<Material>(cubeShader);
    m_modelMaterial = cubeMaterial;

    // 创建渲染组件，组件负责使用Model和Material绘制对象
    auto renderComponent = std::make_unique<RenderComponent>(cubeModel, cubeMaterial);

    // 创建场景对象，并把渲染组件挂到对象上
    auto gameObject = std::make_unique<GameObject>(std::move(renderComponent));

    // 让立方体斜着面对相机，方便确认它是三维模型
    gameObject->GetTransform().rotation = {25.0f, -35.0f, 0.0f};

    // 把对象加入场景，之后由Scene统一绘制
    m_scene.AddGameObject(std::move(gameObject));
}

} // namespace ogl
